 #include "RDA5807M.h"

FmRadio RDA5807;

int8_t init_rda5807() {

  Wire.begin();
  Wire.beginTransmission(RDA5807M_I2C_I_ADDR);
  if (Wire.endTransmission() != 0)
    return -1;

  RDA5807.chipid = RDA5807M_FUNC_CHIPID;
  RDA5807.config = RDA5807M_FUNC_DMIZ | RDA5807M_FUNC_DMUTE | RDA5807M_FUNC_SOFTRESET | RDA5807M_FUNC_ENABLE;
  RDA5807.tune   = RDA5807M_FUNC_CHANNEL | RDA5807M_FUNC_TUNE;
  RDA5807.gpio   = RDA5807M_FUNC_RDS_FIFO_CLR;
  RDA5807.volume = RDA5807M_FUNC_INT_MODE | RDA5807M_FUNC_SEEKTH | RDA5807M_FUNC_LNA_PORT_SEL_2 | RDA5807M_FUNC_VOLUME;
  RDA5807.i2s    = RDA5807M_FUNC_RSVD_2;  
  RDA5807.blend  = RDA5807M_FUNC_TH_SOFRBLEND | RDA5807M_FUNC_65M_50M_MODE | RDA5807M_FUNC_SOFTBLEND_EN;
  RDA5807.freq   = RDA5807M_FUNC_FREQ_DIRECT;

  uint16_t data[7];

  data[0] = RDA5807.config;
  data[1] = RDA5807.tune;
  data[2] = RDA5807.gpio;
  data[3] = RDA5807.volume;
  data[4] = RDA5807.i2s;
  data[5] = RDA5807.blend;
  data[7] = RDA5807.freq;

  uint8_t len = sizeof(uint16_t) * 7;
  writeL((uint8_t *)data, len);

  softreset(DISABLE);
  bass(ENABLE);
  volume(VOLUME_MAX);
  channel(FREQ);
  
  return 0;
}

int8_t write8(uint8_t addr, uint8_t data) {

  Wire.beginTransmission(RDA5807M_I2C_I_ADDR);
  Wire.write(addr);
  Wire.write(data);
  
 if (Wire.endTransmission())
    return 0;
  else
    return -1;
}

int8_t write16(uint8_t addr, uint16_t data) {

  Wire.beginTransmission(RDA5807M_I2C_I_ADDR);
  Wire.write(addr);
  Wire.write(data >> 8);
  Wire.write(data & 0xFF);

  if (Wire.endTransmission())
    return 0;
  else
    return -1;
}

int8_t writeL(const uint8_t *data, uint8_t len) {

  Wire.beginTransmission(RDA5807M_I2C_S_ADDR);
  Wire.write(data, len);
  
 if (Wire.endTransmission())
    return 0;
  else
    return -1;
}

/*
  Audio Output High-Z Disable

  0 = High impedance; 1 = Normal operation
*/
void dhiz(uint8_t flag) {

  if (flag)
    RDA5807.config |= RDA5807M_FUNC_DMIZ;
  else
    RDA5807.config &= (~RDA5807M_FUNC_DMIZ);

  write16(RDA5807M_REG_CONFIG, RDA5807.config);
}

/*
  Mute Disable

  0 = Mute; 1 = Normal operation
*/
void dmute(uint8_t flag) {

  if (flag)
    RDA5807.config |= RDA5807M_FUNC_DMUTE;
  else
    RDA5807.config &= (~RDA5807M_FUNC_DMUTE);

  write16(RDA5807M_REG_CONFIG, RDA5807.config);
}

/*
  Mono Select

  0 = Stereo; 1 = Force mono
*/
void mono(uint8_t flag) {

  if (flag)
    RDA5807.config |= RDA5807M_FUNC_MONO;
  else
    RDA5807.config &= (~RDA5807M_FUNC_MONO);

  write16(RDA5807M_REG_CONFIG, RDA5807.config);
}

/*
  Bass Boost

  0 = Disabled; 1 = Bass boost enabled
*/
void bass(uint8_t flag) {

  if (flag)
    RDA5807.config |= RDA5807M_FUNC_BASS;
  else
    RDA5807.config &= (~RDA5807M_FUNC_BASS);

  write16(RDA5807M_REG_CONFIG, RDA5807.config);
}

/*
  0 = RCLK clock is always supply
  1 = RCLK clock is not always supply when FM
      work ( when 1, RDA5807M can’t directly
      support -20 °C ~ 70°C temperature.
      Only suppory ±20 °C temperature swing from tune point)
*/
void rclknoncal(uint8_t flag) {

  if (flag)
    RDA5807.config |= RDA5807M_FUNC_RCLKNONCAL;
  else
    RDA5807.config &= (~RDA5807M_FUNC_RCLKNONCAL);

  write16(RDA5807M_REG_CONFIG, RDA5807.config);
}

/*
  1 = RCLK clock use the directly input mode
*/
void rclkdirect(uint8_t flag) {

  if (flag)
    RDA5807.config |= RDA5807M_FUNC_RCLKDIRECT;
  else
    RDA5807.config &= (~RDA5807M_FUNC_RCLKDIRECT);

  write16(RDA5807M_REG_CONFIG, RDA5807.config);
}

/*
  Seek Up

  0 = Seek down; 1 = Seek up
*/
void seekup(uint8_t flag) {

  if (flag)
    RDA5807.config |= RDA5807M_FUNC_SEEKUP;
  else
    RDA5807.config &= (~RDA5807M_FUNC_SEEKUP);

  write16(RDA5807M_REG_CONFIG, RDA5807.config);
}

/*
  Seek

  0 = Disable stop seek; 1 = Enable
  Seek begins in the direction specified by
  SEEKUP and ends when a channel is found,
  or the entire band has been searched.
  The SEEK bit is set low and the STC bit is set
  high when the seek operation completes.
*/
void seek(uint8_t flag) {

  if (flag)
    RDA5807.config |= RDA5807M_FUNC_SEEK;
  else
    RDA5807.config &= (~RDA5807M_FUNC_SEEK);

  write16(RDA5807M_REG_CONFIG, RDA5807.config);
}

/*
  Seek Mode

  0 = wrap at the upper or lower band limit and
  continue seeking
  1 = stop seeking at the upper or lower band limit
*/
void skmode(uint8_t flag) {

  if (flag)
    RDA5807.config |= RDA5807M_FUNC_SKMODE;
  else
    RDA5807.config &= (~RDA5807M_FUNC_SKMODE);

  write16(RDA5807M_REG_CONFIG, RDA5807.config);
}

/*
  000=32.768kHz
  001=12Mhz
  101=24Mhz
  010=13Mhz
  110=26Mhz
  011=19.2Mhz
  111=38.4Mhz

*/
void clkmode(uint8_t clk) {

  RDA5807.config &= (~RDA5807M_FUNC_CLKMODE_38MHz);
  switch(clk)
  {
    case 0:
      RDA5807.config |= RDA5807M_FUNC_CLKMODE_32kHz;
      break;
    case 1:
      RDA5807.config |= RDA5807M_FUNC_CLKMODE_12MHz;
      break;
    case 2:
      RDA5807.config |= RDA5807M_FUNC_CLKMODE_24MHz;
      break;
    case 3:
      RDA5807.config |= RDA5807M_FUNC_CLKMODE_13MHz;
      break;
    case 4:
      RDA5807.config |= RDA5807M_FUNC_CLKMODE_26MHz;
      break;
    case 5:
      RDA5807.config |= RDA5807M_FUNC_CLKMODE_19MHz;
      break;
    case 6:
      RDA5807.config |= RDA5807M_FUNC_CLKMODE_38MHz;
      break;
    default:
      RDA5807.config |= RDA5807M_FUNC_CLKMODE_32kHz;
      break;
  }
  write16(RDA5807M_REG_CONFIG, RDA5807.config);
}

/*
  RDS/RBDS enable

  If 1, rds/rbds enable
*/
void rdsen(uint8_t flag) {

  if (flag)
    RDA5807.config |= RDA5807M_FUNC_RDSEN;
  else
    RDA5807.config &= (~RDA5807M_FUNC_RDSEN);

  write16(RDA5807M_REG_CONFIG, RDA5807.config);
}

/*
  Soft reset

  If 0, not reset.
  If 1, reset.
*/
void softreset(uint8_t flag) {

  if (flag)
    RDA5807.config |= RDA5807M_FUNC_SOFTRESET;
  else
    RDA5807.config &= (~RDA5807M_FUNC_SOFTRESET);

  write16(RDA5807M_REG_CONFIG, RDA5807.config);
}

/*
  Power Up Enable

  0 = Disabled; 1 = Enabled
*/
void enable(uint8_t flag) {

  if (flag)
    RDA5807.config |= RDA5807M_FUNC_ENABLE;
  else
    RDA5807.config &= (~RDA5807M_FUNC_ENABLE);

  write16(RDA5807M_REG_CONFIG, RDA5807.config);
}

/*
  Channel Select

  BAND = 0
  Frequency = Channel Spacing (kHz) x CHAN + 87.0 MHz

  BAND = 1 or 2
  Frequency = Channel Spacing (kHz) x CHAN + 76.0 MHz

  BAND = 3
  Frequency = Channel Spacing (kHz) x CHAN + 65.0 MHz
  CHAN is updated after a seek operation.
*/
void channel(float freq) {

  uint16_t val = 0;
  uint8_t four = RDA5807.tune & 0x000F;

  if ((four >> 2) == 0)
  {
    if ( (four & 0x03) == 0 )
      val = (10 * freq - 870) / 1;
    else if ( (four & 0x03) == 1 )
      val = (10 * freq - 870) / 0.5;
    else if ( (four & 0x03) == 2 )
      val = (10 * freq - 870) / 2;
    else
      val = (10 * freq - 870) / 0.25;

  }
  else if ((four >> 2) == 3)
  {
    if ( (four & 0x03) == 0 )
      val = (10 * freq - 650) / 1;
    else if ( (four & 0x03) == 1 )
      val = (10 * freq - 650) / 0.5;
    else if ( (four & 0x03) == 2 )
      val = (10 * freq - 650) / 2;
    else
      val = (10 * freq - 650) / 0.25;
  }
  else
  {
    if ( (four & 0x03) == 0 )
      val = (freq - 760) / 1;
    else if ( (four & 0x03) == 1 )
      val = (freq - 760) / 0.5;
    else if ( (four & 0x03) == 2 )
      val = (freq - 760) / 2;
    else
      val = (freq - 760) / 0.25;
  }

  RDA5807.tune = (val << 6) | (RDA5807.tune & 0x003F);
  write16(RDA5807M_REG_TUNING, RDA5807.tune);

  _delay_ms(50);
}

/*
  Tune

  0 = Disable
  1 = Enable

  The tune operation begins when the TUNE bit
  is set high. The STC bit is set high when the
  tune operation completes.
  The tune bit is reset to low automatically when
  the tune operation completes..

*/
void tune(uint8_t flag) {

  if (flag)
    RDA5807.tune |= RDA5807M_FUNC_TUNE;
  else
    RDA5807.tune &= (~RDA5807M_FUNC_TUNE);

  write16(RDA5807M_REG_TUNING, RDA5807.tune);
}

/*
  Band Select

  00 = 87–108 MHz (US/Europe)
  01 = 76–91 MHz (Japan)
  10 = 76–108 MHz (world wide)
  11* = 65–76 MHz (East Europe) or 50-65MHz
*/
void band(uint8_t b) {

  RDA5807.tune &= (~RDA5807M_FUNC_BAND_3);
  switch(b)
  {
    case 0:
      RDA5807.tune |= RDA5807M_FUNC_BAND_0;
      break;
    case 1:
      RDA5807.tune |= RDA5807M_FUNC_BAND_1;
      break;
    case 2:
      RDA5807.tune |= RDA5807M_FUNC_BAND_2;
      break;
    case 3:
      RDA5807.tune |= RDA5807M_FUNC_BAND_3;
      break;
    default:
      RDA5807.tune |= RDA5807M_FUNC_BAND_0;
      break;
  }

  write16(RDA5807M_REG_TUNING, RDA5807.tune);
}

/*
  Channel Spacing

  00 = 100 kHz
  01 = 200 kHz
  10 = 50kHz
  11 = 25KHz
*/
void space(uint8_t s) {

  RDA5807.tune &= (~RDA5807M_FUNC_SPACE_25kHz);
  switch(s)
  {
    case 0:
      RDA5807.tune |= RDA5807M_FUNC_SPACE_100kHz;
      break;
    case 1:
      RDA5807.tune |= RDA5807M_FUNC_SPACE_200kHz;
      break;
    case 2:
      RDA5807.tune |= RDA5807M_FUNC_SPACE_50kHz;
      break;
    case 3:
      RDA5807.tune |= RDA5807M_FUNC_SPACE_25kHz;
      break;
    default:
      RDA5807.tune |= RDA5807M_FUNC_SPACE_100kHz;
      break;
  }

  write16(RDA5807M_REG_TUNING, RDA5807.tune);
}

/*
  DAC Gain Control Bits (Volume)

  0000 = min; 1111 = max
  Volume scale is logarithmic
  When 0000, output mute and output
  impedance is very large
*/
void volume(uint16_t level) {

  if (level > 15)
    level = 15;

  RDA5807.volume &= (~RDA5807M_FUNC_VOLUME);
  RDA5807.volume |= level;
  write16(RDA5807M_REG_VOLUME, RDA5807.volume);
}