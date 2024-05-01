#include "main.h"

void setup() {
  Serial.begin(9600);
  configuraPinOut();
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  pin_trigger_TRIAC.value = HIGH;
  pinModel_setPinValue(&pin_trigger_TRIAC);
  delay(1000);
  pin_trigger_TRIAC.value = LOW;
  pinModel_setPinValue(&pin_trigger_TRIAC);
}