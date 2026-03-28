
// PID Parameters
float Kp = 1.5; 
float Ki = 0.0000001; 
float Kd = 4.5; 
int16_t lasterror = 0;

int32_t P,I = 0,D;
bool isRunning = false; // The robot starts in a "Wait" state
uint16_t blackThreshold;

// Definition for Speed Control
const uint8_t maxspeeda = 105;
const uint8_t maxspeedb = 105;
uint8_t basespeeda = 65;
uint8_t basespeedb = 65;
uint8_t SensorCount = 8;
uint8_t sensor[] = { A0, A1, A2, A3, A4, A5, A6, A7};
uint16_t sensor_values[8];
float weights[] = {-3500, -2500, -1500, -500, 500, 1500, 2500, 3500};

// Motor Pin Definitions
#define AIN1 3
#define AIN2 4
#define PWMA 5
#define BIN1 7
#define BIN2 8
#define PWMB 6
#define STBY 2

#define left_threshold -995
#define right_threshold 995
void setup(){

  pinMode(AIN1, OUTPUT);pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);pinMode(BIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  for(uint8_t i = 0;i < SensorCount;i++){
    pinMode(sensor[i], INPUT);
  }

  calibration();
}

void loop(){
  if(isRunning)
    PID_control();
  else
    stop();
}

void calibration(){

  digitalWrite(LED_BUILTIN, HIGH); 
  unsigned long calStart = millis();
  uint16_t minVal = 1023;
  uint16_t maxVal = 0;

  while(millis() - calStart < 4000){
    for(uint8_t i=0;i< SensorCount;i++){
      int32_t val = analogRead(sensor[i]);
      if( val > maxVal) maxVal = val;
      if( val < minVal) minVal = val;
    }
    blackThreshold = (minVal + maxVal)/2;
    if(maxVal - minVal < 100) blackThreshold = 500;
  }
  
  isRunning = true;
  digitalWrite(LED_BUILTIN, LOW);
  return;
}

void forward(int16_t speeda,int16_t speedb){
  digitalWrite(AIN1, HIGH);digitalWrite(AIN2, LOW);
  analogWrite(PWMA,speeda);
  digitalWrite(BIN1, HIGH);digitalWrite(BIN2, LOW);
  analogWrite(PWMB,speedb);  
}

void left(){
    digitalWrite(AIN1, HIGH);digitalWrite(AIN2, LOW);
    analogWrite(PWMA,10);
    digitalWrite(BIN1,HIGH);digitalWrite(BIN2, LOW);
    analogWrite(PWMB,basespeeda+50);
}

void right(){
    digitalWrite(AIN1, HIGH);digitalWrite(AIN2, LOW);
    analogWrite(PWMA,basespeeda+50);
    digitalWrite(BIN1, HIGH);digitalWrite(BIN2, LOW);
    analogWrite(PWMB,10);
}

void stop(){
  digitalWrite(AIN1, LOW);digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);digitalWrite(BIN2, LOW);
}

int16_t posit(){
    int32_t weighted_sum = 0;
    int16_t active_sensors = 0;

    for(uint8_t i=0;i < SensorCount; i++){
      sensor_values[i] = analogRead(sensor[i]);
      if(sensor_values[i] > blackThreshold){
        weighted_sum += (sensor_values[i] * weights[i]);
        active_sensors++;
      }
    }
    // IF NO SENSORS see the line, return a special error code
    if(active_sensors == 0) {
       return -32000; // Special code for "LOST"
    }
    return weighted_sum / activeSensors;
}

void PID_control(){
  
  int16_t position = posit();
  if(position == -32000){
    stop();
    delay(100);

    if(lasterror > 0){
      while(sensor_values[4] - left_threshold < 10  && sensor_values[5] - left_threshold < 10){
        sensor_values[4] = analogRead(sensor[2]);
        sensor_values[5] = analogRead(sensor[3]);
        left();
      }
    }
    else if(lasterror < 0){
      while(sensor_values[0] + right_threshold < 10 && sensor_values[1] + right_threshold < 10){
        sensor_values[2] = analogRead(sensor[2]);
        sensor_values[3] = analogRead(sensor[3]);
        right();
      }

    stop();
    delay(50);
    return;
    }
  }

  if(sensor_values[4] - left_threshold < 10  && sensor_values[5] - left_threshold < 10) left();
  if(sensor_values[0] + right_threshold < 10 && sensor_values[1] + right_threshold < 10) right();
  int16_t error = position - 0;  // Setpoint is changed due to change in weights 

  P = error;
  I = I + error;
  D = error - lasterror;

  int32_t motorspeed = P*Kp + I*Ki + D*Kd;
  lasterror = error;

  int32_t motorspeeda = basespeeda + motorspeed;
  int32_t motorspeedb = basespeedb - motorspeed;

  // Constrain For MotorSpeed 
  motorspeeda = constrain(motorspeeda, 0, 255);
  motorspeedb = constrain(motorspeedb, 0, 255);

  if(position >= -470  && position <= -440)
    forward(maxspeeda,maxspeedb);
  else
    forward(motorspeeda,motorspeedb);
  delay(100);
}
