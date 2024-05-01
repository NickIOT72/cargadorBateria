#ifndef SELECTOR_H
#define SELECTOR_H

#include <Arduino.h>

struct Selector
{
  uint8_t pinOrder[4];// Pin order to map the input data on table
  int Table[16];// Table to map the final reuslt
  uint8_t actualPosition;
  uint8_t sizePinOrder;
};

int Selector_init( const struct Selector *sel );
int Selector_mapInput( struct Selector *sel);
void Selector_error( int error );

#endif