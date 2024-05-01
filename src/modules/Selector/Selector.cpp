#include "Selector.h"

#define ERROR_NOPINSCONFIGURED 0 
#define ERROR_PINnotConfigured 1

int Selector_init( const struct Selector *sel  ){
  int err = -1;
  if (  sel->sizePinOrder <= 0 ){
    return ERROR_NOPINSCONFIGURED;
  }
  for(  int i = 0; i < sel->sizePinOrder; i++ )
  {
    if( sel->pinOrder[i] <= 0 ) return ERROR_PINnotConfigured;
    pinMode( sel->pinOrder[i] , INPUT_PULLUP );
  }
  return err;
}

int Selector_mapInput( struct Selector *sel)
{
  int err = -1;
  uint8_t value = 0;
  for(  int i = 0; i < sel->sizePinOrder; i++ )
  {
    value |= ( digitalRead(sel->pinOrder[i]) )<<i;
  }
  sel->actualPosition = value;
  return err;
}

void Selector_error( int error )
{
  if ( error == ERROR_NOPINSCONFIGURED )
  {
    Serial.println( F( "No pins were configured" ) );
  }
  else if ( error == ERROR_PINnotConfigured )
  {
    Serial.println( F( "A pin was no configured" ) );
  }
}