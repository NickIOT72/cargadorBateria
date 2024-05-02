#include <Arduino.h>
#include "modules/Selector/Selector.h"
#include "lib/pinModel/pinModel.h"
#include "lib/ledModel/ledModel.h"

#include "PinChangeInterrupt.h"
// put function declarations here:
void configuraPinOut();
void zerocrossdectectorFunc();

#define SV1_PIN PD3
#define SV2_PIN PD4

#define TABLE_48VDC 48
#define TABLE_36VDC 36
#define TABLE_24VDC 24
#define TABLE_12VDC 12


#define TRIGGER_TRIAC_PIN PD5 
#define ZEROCROSSDETECTOR_PIN PD2
#define LEDSTATUSCARGA_PIN PD7

struct Selector selectorVoltaje;// Selector de voltaje
struct pinModel pin_trigger_TRIAC;// disparador para el triac
struct pinModel pin_zerocrossdetector;// detector de cruce por cero
struct ledModel pin_ledStatusCarga; // Led status de carga;

int countdetector = 0;
void zerocrossdectectorFunc( )
{
  countdetector++;
  if(countdetector % 2 == 0)
  {
    Serial.println(F("Detection"));
  }
  digitalWrite( TRIGGER_TRIAC_PIN, !digitalRead(TRIGGER_TRIAC_PIN) );
}

void configuraPinOut()
{
  int error = -1;

  //********* Selector *****************////
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


  /************************* Pin trigger TRIAC *****************/
  pin_trigger_TRIAC.pinNumber = TRIGGER_TRIAC_PIN;
  pin_trigger_TRIAC.Mode = OUTPUT;
  pin_trigger_TRIAC.type = TYPE_WRITE_DIGITAL;
  pin_trigger_TRIAC.value = 0;
  pin_trigger_TRIAC.interruption = INTERRUPTION_NONE;

  pinModel_init(&pin_trigger_TRIAC);

  /************************* Pin Zero Cross Detector *****************/
  pin_zerocrossdetector.pinNumber = ZEROCROSSDETECTOR_PIN;
  pin_zerocrossdetector.Mode = INPUT_PULLUP;
  pin_zerocrossdetector.type = TYPE_READ_DIGITAL;
  pin_zerocrossdetector.interruption = INTERRUPTION_ATTACH;
  pin_zerocrossdetector.flag_interruption = RISING;
  pin_zerocrossdetector.void_interruption = zerocrossdectectorFunc;
  pinModel_init(&pin_zerocrossdetector);
  attachInterrupt(digitalPinToInterrupt(ZEROCROSSDETECTOR_PIN),zerocrossdectectorFunc, RISING);

  /************************** Pin Led Status Carga *************************/
  pin_ledStatusCarga.pm.pinNumber = LEDSTATUSCARGA_PIN;
  pin_ledStatusCarga.pm.Mode = OUTPUT;
  pin_ledStatusCarga.pm.type = TYPE_WRITE_DIGITAL;
  pin_ledStatusCarga.pm.interruption = INTERRUPTION_NONE;
  pin_ledStatusCarga.pm.value = 0;
  pin_ledStatusCarga.delayOFF = 50;
  pin_ledStatusCarga.delayON = 50;
  ledModel_init( &pin_ledStatusCarga );

}