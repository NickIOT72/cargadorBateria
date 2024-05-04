#include "main.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  configuraPinOut();
  buttonModel_initCountTime(&switch_encendido);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonModel_verifyCountTime(&switch_encendido);
  if ( switch_encendido.lastvalue == LOW  && switch_encendido.counterPressed >= 1 && !switch_encendido.actualState )
  {
    ledModel_ON(&pin_ledStatusCarga);
  }
  else if (switch_encendido.lastvalue == HIGH && switch_encendido.counterNoPressed >= 1 && !switch_encendido.actualState )
  {
    ledModel_OFF(&pin_ledStatusCarga);
  }
  delay(50);
}