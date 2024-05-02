#ifndef PINMODEL_H
#define PINMODEL_H

#include <Arduino.h>

#define TYPE_READ_NONE 0 
#define TYPE_READ_DIGITAL 1
#define TYPE_READ_ANALOG 2
#define TYPE_WRITE_DIGITAL 3
#define TYPE_WRITE_ANALOG 4

#define INTERRUPTION_NONE 0
#define INTERRUPTION_ATTACH 1
#define INTERRUPTION_PCINT 2

struct pinModel
{
  uint8_t pinNumber;
  uint8_t Mode;
  uint8_t type;
  uint16_t value;
  uint8_t interruption;
  uint8_t flag_interruption;
  void (*void_interruption)();
};

int pinModel_init( struct pinModel *pm );
int pinModel_setPinValue( struct pinModel *pm );
int pinModel_getPinValue( struct pinModel *pm );

#endif