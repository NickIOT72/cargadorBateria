#include "main.h"

void setup() {
  Serial.begin(9600);
  configuraPinOut();
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  pinModel_getPinValue(&pin_pot_carga);
  adc_pot_carga =  pin_pot_carga.value<=FROMMIN_POTCARGA?TOMIN_POTCARGA:  pin_pot_carga.value>=FROMMAX_POTCARGA?TOMAX_POTCARGA: map( pin_pot_carga.value, FROMMIN_POTCARGA, FROMMAX_POTCARGA, TOMIN_POTCARGA, TOMAX_POTCARGA);
  Serial.print("Volt: ");
  Serial.print(pin_pot_carga.value);
  Serial.print(" , ");
  Serial.print(adc_pot_carga);
  Serial.println(" Vdc");
  delay(1000);
}