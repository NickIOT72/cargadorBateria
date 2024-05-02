#include "ledModel.h"

int ledModel_init( struct ledModel *lm  )
{
  int err = -1;
  err = pinModel_init( &lm->pm);
  lm->tsart = 0;
  lm->CountHIGH = 0;
  lm->CountLOW = 0;
  lm->allowBlink = false;
  return err;
}

int ledModel_ON(struct ledModel *lm )
{
  int err = -1;
  lm->pm.value = HIGH;
  err = pinModel_setPinValue(&lm->pm);
  lm->CountHIGH++;
  return err;
}

int ledModel_OFF(struct ledModel *lm )
{
  int err = -1;
  lm->pm.value = LOW;
  err = pinModel_setPinValue(&lm->pm);
  lm->CountLOW++;
  return err;
}

int ledModel_Toggle(struct ledModel *lm )
{
  int err = -1;
  lm->pm.value = !lm->pm.value;
  err = pinModel_setPinValue(&lm->pm);
  lm->pm.value ?  lm->CountHIGH++:lm->CountLOW++;
  return err;
}

int ledModel_setPWM(struct ledModel *lm )
{
  int err = -1;
  if ( lm->pm.type != TYPE_WRITE_DIGITAL )
  {
    return 0;
  }
  err = pinModel_setPinValue(&lm->pm);
  return err;
}

int ledModel_verifyBlink(struct ledModel *lm )
{
  int err = -1;
  if (  lm->pm.type == TYPE_WRITE_DIGITAL && lm->allowBlink )
  {
    if ( lm->pm.value == LOW )
    {
      if ( millis() >= lm->tsart + lm->delayOFF  )
      {
        ledModel_ON(lm);
        ledModel_setBlink(lm);
      }
    }
    else if ( lm->pm.value == HIGH )
    {
      if ( millis() >= lm->tsart + lm->delayON  )
      {
        ledModel_OFF(lm);
        ledModel_setBlink(lm);
      }
    }
  }
  else
  {
    return 0;
  }
  return err;
}

int ledModel_quitBlink(struct ledModel *lm )
{
  int err = -1;
  lm->tsart = 0;
  lm->allowBlink = false;
  return err;
}

int ledModel_setBlink(struct ledModel *lm )
{
  int err = -1;
  lm->tsart = millis();
  lm->allowBlink = true;
  return err;
}

int ledModel_resetCount(struct ledModel *lm )
{
  int err = -1;
  lm->CountHIGH = 0;
  lm->CountLOW = 0;
  return err;
}