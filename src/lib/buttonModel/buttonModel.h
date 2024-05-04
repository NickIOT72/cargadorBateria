#ifndef BUTTONMODEL_H
#define BUTTONMODEL_H

#include <Arduino.h>
#include "lib/pinModel/pinModel.h"

struct buttonModel
{
  struct pinModel pm;
  unsigned long tsart;
  uint32_t countTimeNoPressed;
  uint32_t countTimePressed;
  uint32_t counterPressed;
  uint32_t counterNoPressed;
  uint16_t lastvalue;
  bool actualState;
  bool allowCount;
};

int buttonModel_init( struct buttonModel *bm );
int buttonModel_getValue( struct buttonModel *bm );
int buttonModel_initCountTime( struct buttonModel *bm );
int buttonModel_endCountTime( struct buttonModel *bm );
int buttonModel_verifyCountTime( struct buttonModel *bm );
int buttonModel_resetCountTime( struct buttonModel *bm );

#endif