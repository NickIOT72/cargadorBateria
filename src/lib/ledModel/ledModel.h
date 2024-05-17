#ifndef LEDMODEL_H
#define LEDMODEL_H

#include <Arduino.h>
#include "lib/pinModel/pinModel.h"

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
int ledModel_verifyBlinkMicro(struct ledModel *lm );
int ledModel_setPWM(struct ledModel *lm  );
int ledModel_setBlink(struct ledModel *lm );
int ledModel_quitBlink(struct ledModel *lm );
int ledModel_resetCount(struct ledModel *lm );

#endif