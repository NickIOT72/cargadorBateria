#include "buttonModel.h"

int buttonModel_init( struct buttonModel *bm )
{
  int err = -1;
  err = pinModel_init( &bm->pm );
  bm->allowCount=false;
  buttonModel_getValue(bm);
  bm->counterNoPressed = 0;
  bm->counterPressed = 0;
  bm->actualState = false;
  return err;
}

int buttonModel_getValue( struct buttonModel *bm )
{
  int err = -1;
  bm->lastvalue = bm->pm.value;
  err = pinModel_getPinValue( &bm->pm  );
  Serial.println(bm->pm.value);
  if( bm->lastvalue != bm->pm.value ) buttonModel_resetCountTime( bm );
  return err;
}

int buttonModel_initCountTime( struct buttonModel *bm )
{
  int err = -1;
  bm->tsart = millis();
  bm->counterPressed = 0;
  bm->counterNoPressed = 0;
  bm->allowCount = true;
  bm->actualState = false;
  return err;
}

int buttonModel_endCountTime( struct buttonModel *bm )
{
  int err = -1;
  bm->tsart = 0;
  bm->allowCount = false;
  bm->actualState = false;
  return err;
}

int buttonModel_verifyCountTime( struct buttonModel *bm )
{
  int err = -1;
  if (  bm->pm.type == TYPE_READ_DIGITAL && bm->allowCount )
  {
    buttonModel_getValue(bm);
    if ( bm->pm.value == LOW   )
    {
      if ( millis() >= bm->tsart + bm->countTimePressed && bm->actualState )
      {
        bm->counterPressed++;
        bm->actualState = false;
      }
    }
    else if ( bm->pm.value == HIGH )
    {
      if ( millis() >= bm->tsart + bm->countTimeNoPressed && bm->actualState  )
      {
        bm->counterNoPressed++;
        bm->actualState = false;
      }
    }
  }
  else
  {
    return 0;
  }
  return err;
}

int buttonModel_resetCountTime( struct buttonModel *bm )
{
  int err = -1;
  bm->tsart = millis();
  bm->actualState = true;
  Serial.print("State changed:");
  Serial.println( bm->lastvalue, DEC );
  return err;
}