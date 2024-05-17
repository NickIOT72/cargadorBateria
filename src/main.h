#include <Arduino.h>
#include "Convertion.h"
#include "modules/Selector/Selector.h"
#include "lib/pinModel/pinModel.h"
#include "lib/ledModel/ledModel.h"
#include "lib/buttonModel/buttonModel.h"

#include "PinChangeInterrupt.h"
// put function declarations here:
void configuraPinOut();
void zerocrossdectectorFunc();

#define SV1_PIN PD3
#define SV2_PIN PD4
#define TRIGGER_TRIAC_PIN 5 
#define ZEROCROSSDETECTOR_PIN PD2
#define LEDSTATUSCARGA_PIN PD7
#define ADC_VOLT80VDC A0
#define ADC_POTCARGA A1
#define SWITCH_ENCENDIDO_PIN A2
#define LED_STATUS_INV_PIN 8
#define RELAY_TRANSFERENCIA_PIN 9
#define RELAY_TRIAC_PIN PD6
#define HABILITAR_INV_PIN A3

#define TABLE_48VDC 48
#define TABLE_36VDC 36
#define TABLE_24VDC 24
#define TABLE_12VDC 12

struct Selector selectorVoltaje;// Selector de voltaje
struct ledModel pin_trigger_TRIAC;// disparador para el triac
struct pinModel pin_zerocrossdetector;// detector de cruce por cero
struct ledModel pin_ledStatusCarga; // Led status de carga;
struct ledModel pin_ledStatusInv; // Led status de INVERSOR;
struct ledModel relay_transferencia; // relay de cambio de carga (red publica a inversor)
struct pinModel pin_volt_80vdc;// lectura adc de carga de bateria
struct pinModel pin_pot_carga;// lectura adc de carga de bateria
struct buttonModel switch_encendido;// switch de endendido
struct ledModel relay_triac; // relay de actiacion de TRIAC;
struct ledModel pin_habilitar_inversor; // Led status de carga;

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
  pin_trigger_TRIAC.pm.pinNumber = TRIGGER_TRIAC_PIN;
  pin_trigger_TRIAC.pm.Mode = OUTPUT;
  pin_trigger_TRIAC.pm.type = TYPE_WRITE_DIGITAL;
  pin_trigger_TRIAC.pm.value = 0;
  pin_trigger_TRIAC.pm.interruption = INTERRUPTION_NONE;

  pin_trigger_TRIAC.delayOFF = 0;
  pin_trigger_TRIAC.delayON = 0;
  ledModel_init(&pin_trigger_TRIAC);

  /************************* Pin Zero Cross Detector *****************/
  pin_zerocrossdetector.pinNumber = ZEROCROSSDETECTOR_PIN;
  pin_zerocrossdetector.Mode = INPUT_PULLUP;
  pin_zerocrossdetector.type = TYPE_READ_DIGITAL;
  pin_zerocrossdetector.interruption = INTERRUPTION_ATTACH;
  pin_zerocrossdetector.flag_interruption = RISING;
  pin_zerocrossdetector.void_interruption = zerocrossdectectorFunc;
  pinModel_init(&pin_zerocrossdetector);
  attachInterrupt(digitalPinToInterrupt(ZEROCROSSDETECTOR_PIN),zerocrossdectectorFunc, RISING);
  //attachPCINT( digitalPinToPCINT(ZEROCROSSDETECTOR_PIN) , zerocrossdectectorFunc,  RISING );

  /************************** Pin Led Status Carga *************************/
  pin_ledStatusCarga.pm.pinNumber = LEDSTATUSCARGA_PIN;
  pin_ledStatusCarga.pm.Mode = OUTPUT;
  pin_ledStatusCarga.pm.type = TYPE_WRITE_DIGITAL;
  pin_ledStatusCarga.pm.interruption = INTERRUPTION_NONE;
  pin_ledStatusCarga.pm.value = 0;
  pin_ledStatusCarga.delayOFF = 50;
  pin_ledStatusCarga.delayON = 50;
  ledModel_init( &pin_ledStatusCarga );

  /************************** Pin Led Status Inversor *************************/
  pin_ledStatusInv.pm.pinNumber = LED_STATUS_INV_PIN;
  pin_ledStatusInv.pm.Mode = OUTPUT;
  pin_ledStatusInv.pm.type = TYPE_WRITE_DIGITAL;
  pin_ledStatusInv.pm.interruption = INTERRUPTION_NONE;
  pin_ledStatusInv.pm.value = 0;
  pin_ledStatusInv.delayOFF = 0;
  pin_ledStatusInv.delayON = 0;
  ledModel_init( &pin_ledStatusInv );

  /************************** Pin RELAY DE TRANSFERENCIA *************************/
  relay_transferencia.pm.pinNumber = RELAY_TRANSFERENCIA_PIN;
  relay_transferencia.pm.Mode = OUTPUT;
  relay_transferencia.pm.type = TYPE_WRITE_DIGITAL;
  relay_transferencia.pm.interruption = INTERRUPTION_NONE;
  relay_transferencia.pm.value = 0;
  relay_transferencia.delayOFF = 0;
  relay_transferencia.delayON = 0;
  ledModel_init( &relay_transferencia );

  /************************* Pin adc voltimetro 80vdc *****************/
  pin_volt_80vdc.pinNumber = ADC_VOLT80VDC;
  pin_volt_80vdc.Mode = INPUT;
  pin_volt_80vdc.type = TYPE_READ_ANALOG;
  pin_volt_80vdc.interruption = INTERRUPTION_NONE;
  pinModel_init(&pin_volt_80vdc);

  /************************* Pin adc potenciometro carga *****************/
  pin_pot_carga.pinNumber = ADC_POTCARGA;
  pin_pot_carga.Mode = INPUT;
  pin_pot_carga.type = TYPE_READ_ANALOG;
  pin_pot_carga.interruption = INTERRUPTION_NONE;
  pinModel_init(&pin_pot_carga);

  /************************* Pin switch encendido *****************/
  switch_encendido.pm.pinNumber = SWITCH_ENCENDIDO_PIN;
  switch_encendido.pm.Mode = INPUT_PULLUP;
  switch_encendido.pm.type = TYPE_READ_DIGITAL;
  switch_encendido.pm.interruption = INTERRUPTION_NONE;
  switch_encendido.pm.value = 0;
  switch_encendido.countTimePressed = 2000;
  buttonModel_init(&switch_encendido);

  /************************** Pin Relay TRIAC *************************/
  relay_triac.pm.pinNumber = RELAY_TRIAC_PIN;
  relay_triac.pm.Mode = OUTPUT;
  relay_triac.pm.type = TYPE_WRITE_DIGITAL;
  relay_triac.pm.interruption = INTERRUPTION_NONE;
  relay_triac.pm.value = 0;
  relay_triac.delayOFF = 0;
  relay_triac.delayON = 0;
  ledModel_init( &relay_triac );

  /************************** Pin Habiliatar Inversor *************************/
  pin_habilitar_inversor.pm.pinNumber = HABILITAR_INV_PIN;
  pin_habilitar_inversor.pm.Mode = OUTPUT;
  pin_habilitar_inversor.pm.type = TYPE_WRITE_DIGITAL;
  pin_habilitar_inversor.pm.interruption = INTERRUPTION_NONE;
  pin_habilitar_inversor.pm.value = 0;
  pin_habilitar_inversor.delayOFF = 0;
  pin_habilitar_inversor.delayON = 0;
  ledModel_init( &pin_habilitar_inversor );
}