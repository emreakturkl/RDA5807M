#include <Arduino.h>
#include "RDA5807M.h"

#define PRINT(x) Serial.print(x)
#define PRINTLN(x) Serial.println(x)

#define FREQ_MIN 87.0
#define FREQ_MAX 108.0
#define FREQ_FAV 106.2

#define DEBUG 0
#define RADIO_STATION 0

int main(void)
{
  init();

  Serial.begin(9600);
  while(init_rda5807() != 0)
    PRINTLN("Device Not Found.");
  PRINTLN("RDA5807M is Found.");

  channel(FREQ_FAV);

#if DEBUG
  uint32_t data;
  uint16_t info[2];
  data = info_rda5807();

  info[0] = data & 0x0000FFFF;
  info[1] = data >> 16;

  PRINT("RSSI: ");
  PRINTLN(info[1] & 0xFE00);
  (info[0] & 0x0100) ? PRINTLN("The current channel is a station.") : PRINTLN("The current channel is not a station.");
  (info[0] & 0x0080) ? PRINTLN("FM ready.") : PRINTLN("FM not ready.");

  (info[1] & 0x8000) ? PRINTLN("New RDS/RBDS group ready.") : PRINTLN("No RDS/RBDS group ready(default)");
  (info[1] & 0x4000) ? PRINTLN("Seek/Tune Complete.") : PRINTLN("Seek/Tune Not complete.");
  (info[1] & 0x2000) ? PRINTLN("Seek successful.") : PRINTLN("Seek failure.");
  (info[1] & 0x1000) ? PRINTLN("RDS decoder synchronized.") : PRINTLN("RDS decoder not synchronized(default).");
  (info[1] & 0x0800) ? PRINTLN("When RDS enable: Block E has been found.") : PRINTLN("When RDS enable: no Block E has been found.");
  (info[1] & 0x0400) ? PRINTLN("Stereo Indicator: Stereo.") : PRINTLN("Stereo Indicator: Mono.");
  PRINT("Channel: ");
  PRINTLN(info[1] & 0x02FF);
#endif

#if RADIO_STATION
  for(float i = FREQ_MIN; i <= FREQ_MAX; i += 0.1)
  {
    channel(i);
    PRINT("Channel: ");  
    PRINT(i);
    PRINTLN(" MHz"); 
    _delay_ms(2000);
  }
#endif
  while (1) 
  {
    PRINT(".");
    _delay_ms(10000);
  }
  
  return 0;
}

