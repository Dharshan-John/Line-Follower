
uint16_t blackThreshold;
int16_t position;
uint8_t SensorCount = 8;
uint8_t sensor[] = { A0, A1, A2, A3, A4, A5, A6, A7};
uint16_t sensor_values[6];
uint16_t calibration_values[];
float weights[] = {-3.75,-2.5,-1.5,0.001,0.001,0,1.5,2.5,3.75};

void setup(){

  pinMode(LED_BUILTIN, OUTPUT);
  for(uint8_t i = 0;i < SensorCount;i++){
    pinMode(sensor[i], INPUT);
  }

  Serial.begin(9600);
  Serial.println("Calibration Started");
  calibration();
}

void loop(){
  float weighted_sum = 0;
  uint16_t active_sensors = 0;

  for(uint8_t i=0;i < SensorCount; i++){
    sensor_values[i] = analogRead(sensor[i]);
    if(sensor_values[i] > blackThreshold){
      weighted_sum += (sensor_values[i] * weights[i]);
      active_sensors++;
    }
  }

  position = weighted_sum / SensorCount;    
  // 3. Print Values (Corrected Syntax)
  Serial.print("S0:"); Serial.print(sensor_values[0]); Serial.print("\t");
  Serial.print("S1:"); Serial.print(sensor_values[1]); Serial.print("\t");
  Serial.print("S2:"); Serial.print(sensor_values[2]); Serial.print("\t");
  Serial.print("S3:"); Serial.print(sensor_values[3]); Serial.print("\t");
  Serial.print("S4:"); Serial.print(sensor_values[4]); Serial.print("\t");
  Serial.print("S5:"); Serial.print(sensor_values[5]); Serial.print("\t");
  Serial.print("S6:"); Serial.print(sensor_values[6]); Serial.print("\t");
  Serial.print("S7:"); Serial.print(sensor_values[7]); Serial.print("\t");

  Serial.print(" | POS: "); 
  Serial.println(position); // println at the very end creates a new line
  
  delay(100); // Slow down prints so you can read them
}

uint16_t findMin(uint16_t &arr) {
    uint16_t minVal = arr[0];   // assume first is smallest

    for (uint8_t i = 1; i < SensorCount; i++) {
        if (arr[i] < minVal) {
            minVal = arr[i];
        }
    }
    return minVal;
}

uint16_t 

void calibration(){
  uint16_t minVal = 1023;
  uint16_t maxVal = 0;
  digitalWrite(LED_BUILTIN, HIGH);
  unsigned long calStart = millis();
  while(millis() - calStart < 10000){
    for(uint8_t i=0;i< SensorCount;i++){
      calibration_values[i] = analogRead(sesnor[i]);
      int32_t val = analogRead(sensor[i]);
      if( val > maxVal) maxVal = val;
      if( val < minVal) minVal = val;
    }
  }
  blackThreshold = (minVal + maxVal)/2;
  if(maxVal - minVal < 100) blackThreshold = 500;
  Serial.print("Calibration Completed. BlackThreshold:");
  Serial.println(blackThreshold);

  findMin(calibration_values);
  digitalWrite(LED_BUILTIN, LOW);
}
