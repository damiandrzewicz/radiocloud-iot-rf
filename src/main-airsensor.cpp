#include <Arduino.h>
#include <ArduinoLog.h>
#include <Wire.h>

#include "node/RadioCloudNode.hpp"
#include "SparkFunBME280.h"

class AirSensor : public RadioCloudNode
{
public:

private:
  BME280 bme280_;
};

AirSensor airSensor;

void setup() {
  Serial.begin(115200);

    // put your setup code here, to run once:
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  Log.info(F("main setup"));

  airSensor.setup();
}

void loop() {
  airSensor.loop();
}