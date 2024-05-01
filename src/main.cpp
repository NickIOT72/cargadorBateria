#include "main.h"

void setup() {
  Serial.begin(9600);
  configuraPinOut();
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Selector_mapInput( &selectorVoltaje );
  Serial.print(F( "Selector Input: " ));
  Serial.print( selectorVoltaje.Table[ selectorVoltaje.actualPosition ] );
  Serial.println(F( "VDC" ));
}