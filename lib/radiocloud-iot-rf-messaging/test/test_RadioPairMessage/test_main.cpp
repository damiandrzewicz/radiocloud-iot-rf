#include <unity.h>

#ifndef ARDUINO
#include "../utils/helpers.h"
#endif

#include "test_RadioPairRequest.hpp"
#include "test_RadioPairResponse.hpp"


void process() {
    UNITY_BEGIN();
    test_RadioPairRequest::process();
    test_RadioPairRequest::process();
    UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>
void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);
    
    process();
}

void loop() {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}

#else

int main(int argc, char **argv) {
    process();
    return 0;
}

#endif