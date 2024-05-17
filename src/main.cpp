#include "main.h"

struct deviceStatus {
 bool state;
 uint8_t frequency;
 uint16_t timerelapse;
 bool freqAllow;
} devSt ;

//unsigned long tverf = 0;

bool AllowTrigger = false;
void triggerTRIAC()
{
  if( AllowTrigger )
  {
    if ( micros() > pin_trigger_TRIAC.delayON )
    {
      ledModel_ON(&pin_trigger_TRIAC);
      AllowTrigger = false;
    }
  }
}

float porc_bat = 1;
uint16_t loadCharge = 0;
uint16_t timeLoadCharge = 0;
void verifyLoadCharge()
{
  pinModel_getPinValue(&pin_volt_80vdc);
  loadCharge = map( pin_volt_80vdc.value ,0, 1023, 10, 90);
  loadCharge = map( loadCharge, 10, 90,  0, 100)*porc_bat;
  timeLoadCharge = devSt.timerelapse + map( loadCharge,  0, 100 ,devSt.timerelapse,  0);
}


int countdetector = 0;
uint8_t total_size = 10;
unsigned long tzerodetected = 0; 
void zerocrossdectectorFunc( )
{
  static unsigned long arrayDataMeasure[ 10 ];
  static uint8_t countMeasure = 0;
  static uint8_t inputFrequency = 0;
  unsigned long tnow = micros();
  ledModel_OFF(&pin_trigger_TRIAC);
  if ( tzerodetected == 0 )
  {
    tzerodetected = tnow;
    return;
  }
  if (tnow - tzerodetected >=  40000 )
  {
    devSt.freqAllow = false;
    tzerodetected = 0;
    devSt.timerelapse = 0;
    countMeasure=0;
    for ( int i = 0; i <  total_size; i++ )
    {
      arrayDataMeasure[i] = 0;
    }
    return;

  }
  if( !devSt.freqAllow )
  {
    arrayDataMeasure[countMeasure] = tnow - tzerodetected;
    tzerodetected = tnow;
    countMeasure++;
    if ( countMeasure >= total_size   )
    {
      noInterrupts();
      uint32_t MeasureCount = 0;
      for ( int i = 0; i < total_size ; i++ )
      {
        MeasureCount += arrayDataMeasure[i];
      }
      MeasureCount = MeasureCount/total_size;
      inputFrequency = (1000000/2 )/MeasureCount;
      countMeasure=0;
      for ( int i = 0; i <  total_size; i++ )
      {
        arrayDataMeasure[i] = 0;
      }
      devSt.freqAllow = true;
      Serial.print("Frequency:");
      Serial.print(inputFrequency);
      Serial.println("HZ");
      devSt.timerelapse = (250000)/inputFrequency;
      Serial.print("Time relapse:");
      Serial.print(devSt.timerelapse);
      Serial.println("us");
      verifyLoadCharge();
      pin_trigger_TRIAC.delayON = tnow + timeLoadCharge;
      AllowTrigger = true;
      interrupts();
    }
  }
  else{
    tzerodetected = tnow;
    verifyLoadCharge();
    pin_trigger_TRIAC.delayON = tnow + timeLoadCharge;
    AllowTrigger = true;
  }
  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  configuraPinOut();
  buttonModel_initCountTime(&switch_encendido);
  devSt.state = false;
  //tverf = millis();
}

void loop() {
  buttonModel_verifyCountTime( &switch_encendido );
  static bool InputVoltage = false;
  bool prevInputVoltage = false;
  prevInputVoltage = InputVoltage;
  if ( tzerodetected != 0 )
  {
    InputVoltage = !(micros() >  tzerodetected + 4*devSt.timerelapse);
  }
  if ( switch_encendido.lastvalue == LOW  && switch_encendido.counterPressed >= 1 && switch_encendido.actualState == false )
  {
    Serial.println("Presionado");
    buttonModel_endCountTime( &switch_encendido);
    buttonModel_initCountTime( &switch_encendido);
    devSt.state = !devSt.state;
    if (  devSt.state)
    {
      InputVoltage ? ledModel_ON(&relay_transferencia) : ledModel_OFF(&relay_transferencia);
      InputVoltage ? ledModel_ON(&relay_triac) : ledModel_OFF(&relay_triac) ;
      InputVoltage ? ledModel_OFF(&pin_habilitar_inversor) :  ledModel_ON(&pin_habilitar_inversor);
    }
    else
    {
      ledModel_OFF(&relay_transferencia);
      ledModel_OFF(&relay_triac);
      ledModel_OFF(&pin_habilitar_inversor);
      devSt.freqAllow = false;
    }
  }
  if ( devSt.state )
  {
    if ( prevInputVoltage != InputVoltage )
    {
      InputVoltage ? ledModel_ON(&relay_transferencia) : ledModel_OFF(&relay_transferencia);
      InputVoltage ? ledModel_ON(&relay_triac) : ledModel_OFF(&relay_triac) ;
      InputVoltage ? ledModel_OFF(&pin_habilitar_inversor) :  ledModel_ON(&pin_habilitar_inversor);
      
    }
    if(InputVoltage )
    {
      triggerTRIAC();
    }
    else{
      if(devSt.freqAllow) devSt.freqAllow = false;
    }
    /*if ( millis() > tverf + 2000 )
    {
      Serial.print("Volt:");
      Serial.println(loadCharge);
      Serial.print("timeLoadCharge:");
      Serial.println(timeLoadCharge);
      tverf = millis();
    }*/
  }
  // put your main code here, to run repeatedly:
}