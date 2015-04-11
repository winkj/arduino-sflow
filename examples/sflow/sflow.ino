#include <Wire.h>

#include "sensirionflow.h"

SensirionFlow flow(64);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();  
  Serial.begin(9600);

  // initialize the sensor; Note: call this after Wire.begin() as it 
  // communicates with the sensor on I2C
  flow.init();
}

void loop() {
  float result = flow.readSample();
  
  Serial.print("Flow: ");
  if (result >= 0) {
    Serial.print(" ");
  }
  Serial.print(result, 5);
  Serial.print("\n");
  
  delay(100);
}