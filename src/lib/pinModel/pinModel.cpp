#include "pinModel.h"

#define ERROR_WRONG_ANALOG_PINS 0 
#define ERROR_WRONGATTACHINTPIN 1
#define ERROR_WRONGPCINTPIN 2
#define ERROR_WRONGTYPEOFINT 3

#define ERROR_SETPIN_NOTOUTPUT 0


int pinModel_init(  struct pinModel *pm )
{
  int err = -1;
  pinMode( pm->pinNumber , pm->Mode);
  if( pm->Mode == OUTPUT )
  {
    digitalWrite( pm->pinNumber, pm->value );
  }
  else{
    if (pm->type_read == TYPE_READ_DIGITAL )
    {
      pm->value = digitalRead( pm->pinNumber );
    }
    else if (pm->type_read == TYPE_READ_ANALOG  )
    {
      #if defined(__AVR_ATmega328P__) 
      if ( pm->pinNumber >= A0 &&  pm->pinNumber <= A7 ){
        pm->value = analogRead( pm->pinNumber );
      }
      else{
        err = ERROR_WRONG_ANALOG_PINS;
        return err;
      }
      #endif 
    }
  }
  if ( pm->interruption )
  {
    if ( pm->interruption == INTERRUPTION_ATTACH )
    {
      #if defined(__AVR_ATmega328P__) 
      if ( pm->pinNumber == PD2 &&  pm->pinNumber == PD3 ){
        int h = 0;
        //attachInterrupt(digitalPinToInterrupt(pm->pinNumber), pm->void_interruption , pm->flag_interruption);
      }
      else{
        err = ERROR_WRONGATTACHINTPIN;
        return err;
      }
      #endif
    }
    else if( pm->interruption == INTERRUPTION_PCINT )
    {
      #if defined(__AVR_ATmega328P__) 
      if ( true ){
        int h = 0;
        //attachPCINT(digitalPinToPCINT( pm->pinNumber ), pm->void_interruption, pm->flag_interruption);
        //PcInt::attachInterrupt(pm->pinNumber, pinChanged, pm , pm->flag_interruption);
      }
      else{
        err = ERROR_WRONGPCINTPIN;
        return err;
      }
      #endif
    }
  }
  return err;
}

int pinModel_setPinValue( struct pinModel *pm )
{
  int err = -1;
  if( pm->Mode == OUTPUT )
  {
    digitalWrite( pm->pinNumber, pm->value );
  }
  else 
  {
    err = ERROR_SETPIN_NOTOUTPUT;
  }
  return err;
}

int pinModel_getPinValue( struct pinModel *pm )
{
  int err = -1;
  if( pm->Mode == INPUT ||  pm->Mode == INPUT_PULLUP )
  {
    if (  pm->type_read == TYPE_READ_DIGITAL )
    {
      pm->value = digitalRead( pm->pinNumber );
    }
    else if (  pm->type_read == TYPE_READ_ANALOG )
    {
      pm->value = analogRead( pm->pinNumber);
    }
  }
  else 
  {
    err = ERROR_SETPIN_NOTOUTPUT;
  }
  return err;
}