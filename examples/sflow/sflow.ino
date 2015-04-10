#include <Wire.h>

#include "sensirionflow.h"

SensirionFlow flow(64);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();  
  Serial.begin(9600);
  
  delay(1000); // let serial console settle
  flow.init();
  delay(1000); // let serial console settle
}

void loop() {
    
  float result = flow.readSample();
  
  Serial.print("Flow:\n");
  Serial.print(result, 2);
  Serial.print("\n");

  delay(1000);
}
