#include <Arduino.h>
#include "modules/Selector/Selector.h"
// put function declarations here:

#define SV1_PIN PD3
#define SV2_PIN PD4

#define TABLE_48VDC 48
#define TABLE_36VDC 36
#define TABLE_24VDC 24
#define TABLE_12VDC 12

struct Selector selectorVoltaje;

void configuraPinOut()
{
  int error = -1;
  selectorVoltaje.pinOrder[0] = SV1_PIN;
  selectorVoltaje.pinOrder[1] = SV2_PIN;
  selectorVoltaje.pinOrder[2] = 0;
  selectorVoltaje.pinOrder[3] = 0;

  selectorVoltaje.sizePinOrder = 2;

  selectorVoltaje.Table[0] = TABLE_48VDC;
  selectorVoltaje.Table[1] = TABLE_24VDC;
  selectorVoltaje.Table[2] = TABLE_36VDC;
  selectorVoltaje.Table[3] = TABLE_12VDC;
  for (int i = 4; i < 16; i++)
  {
    selectorVoltaje.Table[i] = 0;
  }

  selectorVoltaje.actualPosition = 0;

  error = Selector_init( &selectorVoltaje );
  if ( error >= 0 )
  {
    Serial.println(F("Error while configuring Selector module"));
    Selector_error( error );
    while (true){ delay(1);}
  }
}