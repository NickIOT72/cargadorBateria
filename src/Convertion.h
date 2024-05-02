//// Define all convertions parameters

#include <Arduino.h>

/// Voltaje 80vdc
#define  FROMMIN_80VDC 0
#define  FROMMAX_80VDC 1023
#define  TOMIN_80VDC 0
#define  TOMAX_80VDC 80

// Potencipmetro Carga
#define  FROMMIN_POTCARGA (int)((0.5*1023)/5)
#define  FROMMAX_POTCARGA (int)((4.5*1023)/5)
#define  TOMIN_POTCARGA 0
#define  TOMAX_POTCARGA 100

uint16_t adc_80vdc_val  =0;
uint16_t adc_pot_carga = 0;