#ifndef LEDMODEL_H
#define LEDMODEL_H

#include <Arduino.h>
#include "lib/pinModel/pinModel.h"

#define TYPE_READ_NONE 0 
#define TYPE_READ_DIGITAL 1
#define TYPE_READ_ANALOG 2

#define INTERRUPTION_NONE 0
#define INTERRUPTION_ATTACH 1
#define INTERRUPTION_PCINT 2

struct ledModel
{
  struct pinModel pm;
  unsigned long tsart;
  uint32_t delayON;
  uint32_t delayOFF;
  uint32_t CountHIGH;
  uint32_t CountLOW;
  bool allowBlink;
};

int ledModel_init( struct ledModel *lm );
int ledModel_ON(struct ledModel *lm );
int ledModel_OFF(struct ledModel *lm );
int ledModel_Toggle(struct ledModel *lm );
int ledModel_verifyBlink(struct ledModel *lm );
int ledModel_setPWM(struct ledModel *lm  );
int ledModel_setBlink(struct ledModel *lm );
int ledModel_quitBlink(struct ledModel *lm );
int ledModel_resetCount(struct ledModel *lm );

#endif