#include <Wire.h>

#include "sensirionflow.h"

SensirionFlow flow(64);
char label[64];

// function prototypes
const char* getDimensionLabel(uint8_t dimension);
const char* getTimeBaseLabel(uint8_t timeBase);
const char* getVolumePressureUnitLabels(uint8_t volumePressureUnit);

void setup() {
  // put your setup code here, to run once:
  Wire.begin();  
  Serial.begin(9600);
  delay(500); // let serial console settle
  
  // initialize the sesnor  
  flow.init();
  
  // - print some information (check datasheet to decode)
  uint8_t dimension = flow.getDimension();
  uint8_t timeBase = flow.getTimeBase();
  uint8_t volumePressureUnit = flow.getVolumePressureUnit();
  
  Serial.print("Dimension: ");
  Serial.print(dimension);
  Serial.print("  Time base: ");
  Serial.print(timeBase);
  Serial.print("  Volume/Pressure unit: ");
  Serial.print(volumePressureUnit);
  Serial.print("\n");
  
  label[0] = '\0';
  strcat(label, getDimensionLabel(dimension));
  strcat(label, getVolumePressureUnitLabels(volumePressureUnit));
  strcat(label, getTimeBaseLabel(timeBase));
}

void loop() {
  float result = flow.readSample();
  
  Serial.print("Flow: ");
  if (result >= 0) {
    Serial.print(" ");
  }
  Serial.print(result, 5);
  Serial.print(" ");
  Serial.print(label);
  Serial.print("\n");
  
  delay(100);
}



// - some helper methods to get string labels from dimension/timebase/unit
const char* getDimensionLabel(uint8_t dimension)
{
  static const int LABEL_COUNT = 14;
  static char* dimensionLabels[LABEL_COUNT] = {
    "?",
    "?",
    "?",
    "n",
    "u",
    "m",
    "c",
    "d",
    "",
    "?",
    "h",
    "k",
    "M",
    "G",
  };
  
  if (dimension > (LABEL_COUNT-1)) {
    return "INV";
  }
  return dimensionLabels[dimension];
}

const char* getTimeBaseLabel(uint8_t timeBase)
{
  static const int LABEL_COUNT = 7;
  static char* timeBaseLabels[LABEL_COUNT] = {
    "",
    "/us",
    "/ms",
    "/s",
    "/min",
    "/hour",
    "/day"
  };
  
  if (timeBase > (LABEL_COUNT-1)) {
    return "INV";
  }
  return timeBaseLabels[timeBase];  
}

const char* getVolumePressureUnitLabels(uint8_t volumePressureUnit)
{  
  static const int LABEL_COUNT = 20;
  static char* volumePressureUnitLabels[LABEL_COUNT] = {
    "l(norm)",
    "l(std)",
    "?",
    "?",
    "?",
    "?",
    "?",
    "?",
    "l(liquid)",
    "gram",
    "?",
    "?",
    "?",
    "?",
    "?",
    "?",    
    "pascal",
    "bar",
    "meter H2O",
    "inch H2)"
  };
  
  if (volumePressureUnit > (LABEL_COUNT-1)) {
    return "INV";
  }
  return volumePressureUnitLabels[volumePressureUnit];
}
