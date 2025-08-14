/*
 * File name: PID_LF_TB6612
 * 
 * Hardware: Arduino Nano
 *           QTR-8A Reflectance Sensor Array
 *           TB6612 Motor Driver
 *           N20 Motors
 *           
 * Description: PID-based line follower robot with TB6612
 * 
 * Author: Harshith Venkatesh
 */

#include <QTRSensors.h>         // QTR sensor library
#include <SparkFun_TB6612.h>    // TB6612 library
#include <SoftwareSerial.h>     // bluetooth built-in module

SoftwareSerial HC05(2,13); //RX,TX
char data; 
/*************************************************************************
* Sensor Array Initialization 
*************************************************************************/
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

/*************************************************************************
* PID Control Variables 
*************************************************************************/
float Kp = 0.07;     // Proportional term
float Ki = 0.02;    // Integral term
float Kd = 0.04;       // Derivative term
int P, I, D;
int lastError = 0;

/*************************************************************************
* Motor Speed and Pin Connections (As Per Your Wiring)
*************************************************************************/
const uint8_t maxspeeda = 150;
const uint8_t maxspeedb = 150;
const uint8_t basespeeda = 100;
const uint8_t basespeedb = 100;

// TB6612 Motor Driver Pins
#define AIN1 7  // Motor A direction pin 1
#define AIN2 8    // Motor A direction pin 2
#define PWMA 6    // Motor A PWM speed control
#define BIN1 3    // Motor B direction pin 1
#define BIN2 4    // Motor B direction pin 2
#define PWMB 5    // Motor B PWM speed control
#define STBY 9    // Standby pin

// Motor Offsets
const int offsetA = 1;
const int offsetB = 1;

// Motor Objects
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

/*************************************************************************
* Button Pins with Internal Pull-Ups
*************************************************************************/
int buttoncalibrate = 11;     // Calibration button
int buttonstart = 10;         // Start/stop button
boolean onoff = false;

/*************************************************************************
* Setup Function
*************************************************************************/
void setup() {
  // QTR sensor setup with your pin connections
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  qtr.setEmitterPin(2);  // QTR LED emitter control pin

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);

  // Use internal pull-up resistors for buttons
  pinMode(buttoncalibrate, INPUT_PULLUP);
  pinMode(buttonstart, INPUT_PULLUP);

  // Calibration process
  boolean Ok = false;
  while (!Ok) {  
    if (digitalRead(buttoncalibrate) == LOW) {  // Button pressed (LOW)
      calibration();
      Ok = true;
    }
  }
  stopMotors(motor1, motor2);  // Stop motors after calibration
  HC05.begin(9600);
  Serial.begin(9600);
}

/*************************************************************************
* Calibration Function
*************************************************************************/
void calibration() {
  digitalWrite(LED_BUILTIN, HIGH);
  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW);
}

/*************************************************************************
* Loop Function
*************************************************************************/
void loop() {
  if (digitalRead(buttonstart) == LOW) {  // Button pressed
    onoff = !onoff;
    if (onoff) {
      delay(1000);  // 1 sec delay when robot starts
    } else {
      delay(50);    // Short delay when robot stops
    }
  }

  if (onoff) {
    PID_control();
  } else {
    stopMotors(motor1, motor2);
  }
  if(HC05.availabe()){
    data=HC05.read();
    if(data=='p1') Kp+=0.05;
    else if(data=='p2') Kp-=0.05;
    else if(data=='d1') Kd+=0.02;
    else if(data=='d2') Kd-=0.02;
    else if(data=='i1') Ki+=0.1;
    else if(data=='i2') Ki-=0.1;
  }
}

/*************************************************************************
* PID Control Function
*************************************************************************/
void PID_control() {
  uint16_t position = qtr.readLineBlack(sensorValues);
  int error = position - 3500;  // Center position is 3500

  P = error;
  I += error;
  D = error - lastError;
  lastError = error;

  int motorspeed = P * Kp + I * Ki + D * Kd;

  int motorspeeda = basespeeda + motorspeed;
  int motorspeedb = basespeedb - motorspeed;

  // Constrain motor speeds
  motorspeeda = constrain(motorspeeda, 0, maxspeeda);
  motorspeedb = constrain(motorspeedb, 0, maxspeedb);

  forward_drive(motorspeeda, motorspeedb);
}

/*************************************************************************
* Motor Control Functions
*************************************************************************/
void forward_drive(int posa, int posb) {
  motor1.drive(posa);
  motor2.drive(posb);
}

// Renamed brake function to avoid conflict
void stopMotors(Motor m1, Motor m2) {
  m1.brake();
  m2.brake();
}
}
