# Important: this repository is deprecated - use https://github.com/Sensirion/arduino-sht instead!


# arduino-sflow

Unofficial Arduino driver support for Sensirion flow sensor products. At this point, only sensors using an I2C interface are supported.


## Installation

Download arduino-sflow either via git or from the releases page and place it in
the Arduino/libraries directory. After restarting the Arduino IDE, you will get
menu items under libraries and examples.

## Integrating it into your sketch

Assuming you installed the library as described above, the following steps are necessary:

1. Import the Wire library like this: From the menu bar, select Sketch > Import Library > Wire
1. Import the arduino-sflow library like this: From the menu bar, select Sketch > Import Library > arduino-sflow
1. Create an instance of the `SensirionFlow` class, with the I2C address of the sensor as parameter (check datasheet)
2. In `setup()`, make sure to init the Wire library with `Wire.init()`
3. If you want to use the serial console, remember to initialize the Serial library with `Serial.begin(9600)`
1. Finally, call `flow.readSample()` in the `loop()` function, which returns a float value of the current flow

### Sample code
```c++
#include <Wire.h>

#include <sensirionflow.h>

SensirionFlow flow(64);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();  
  Serial.begin(9600);

  flow.init();
}

void loop() {    
  float result = flow.readSample();
  Serial.print("Flow: ");
  Serial.print(result, 2);
  Serial.print("\n");

  delay(1000);
}
```

