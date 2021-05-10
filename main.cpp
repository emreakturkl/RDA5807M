#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
#include "RDA5807M.h"

#define PRINT(x) Serial.println(x);

#define FREQ_MIN 87.0
#define FREQ_MAX 108.0
#define FREQ_FAV 88.0

#define RADIO_STATION_SEARCH 0

int main(void)
{
  init();

  Serial.begin(9600);
  init_rda5807();
  channel(FREQ_FAV);

 #if RADIO_STATION_SEARCH
  for(float i = FREQ_MIN; i <= FREQ_MAX; i += 0.1)
  {
    channel(i);
    Serial.print("Channel: ");  
    Serial.print(i);
    Serial.println(" MHz"); 
    _delay_ms(2000);
  }
#endif
  while (1) 
  {
    PRINT(".");
    _delay_ms(1000);
  }
  
  return 0;
}

