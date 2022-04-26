#include <Arduino.h>
#include <RTCZero.h>

static constexpr uint8_t ledPin = 13;

RTCZero rtc;

void alarmMatch()
{
  //rtc.setSeconds(0);
}

void blink()
{

}

void setup() {
  // put your setup code here, to run once:

  pinMode(ledPin, OUTPUT);

  for(uint8_t i = 0; i < 15; i++)
  {
    digitalWrite(ledPin, !digitalRead(ledPin));
    delay(200);
  }

  digitalWrite(ledPin, LOW);

  attachInterrupt(digitalPinToInterrupt(7), blink, LOW);

  rtc.begin();

  rtc.setAlarmSeconds(10);

  //rtc.setAlarmTime()
  rtc.enableAlarm(rtc.MATCH_SS);
  rtc.attachInterrupt(alarmMatch);
  rtc.standbyMode();
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(ledPin, !digitalRead(ledPin));
  rtc.standbyMode();
}