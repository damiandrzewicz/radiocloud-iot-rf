#include <Arduino.h>

#include <TrueRandom.h>

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
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

}