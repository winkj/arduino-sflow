#include <Wire.h>

#include "sensirionflow.h"

SensirionFlow flow(64);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();  
  Serial.begin(9600);
  

  // - NOTE: typically, the flow sensor has to be
  // initialized, however since this is still a debug version
  // refrain from doing so for now to get raw sensor data

  // delay(1000); // let serial console settle
  // flow.init();
  // delay(1000); // let serial console settle
}

void loop() {
    
  float result = flow.readSample();
  
  Serial.print("Flow:\n");
  Serial.print(result, 2);
  Serial.print("\n");

  delay(1000);
}
