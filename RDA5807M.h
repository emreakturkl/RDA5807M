#ifndef RDA5807M
#define RDA5807M

#include <util/delay.h>
#include <stdint.h>
#include <Wire.h>

// Default Addresses
#define RDA5807M_I2C_S_ADDR   0x10
#define RDA5807M_I2C_I_ADDR   0x11

// Register Addresses
#define RDA5807M_REG_CHIPID 0x00
#define RDA5807M_REG_CONFIG 0x02
#define RDA5807M_REG_TUNING 0x03
#define RDA5807M_REG_GPIO   0x04
#define RDA5807M_REG_VOLUME 0x05
#define RDA5807M_REG_I2S    0x06
#define RDA5807M_REG_BLEND  0x07
#define RDA5807M_REG_FREQ   0x08
#define RDA5807M_REG_STATUS 0x0A
#define RDA5807M_REG_RSSI   0x0B
#define RDA5807M_REG_RDSA   0x0C
#define RDA5807M_REG_RDSB   0x0D
#define RDA5807M_REG_RDSC   0x0E
#define RDA5807M_REG_RDSD   0x0F

// 0x00H Default
#define RDA5807M_FUNC_CHIPID          0x5804

// 0x02H Functions (Bit Enable) 
#define RDA5807M_FUNC_DMIZ            0x8000
#define RDA5807M_FUNC_DMUTE           0x4000
#define RDA5807M_FUNC_MONO            0x2000
#define RDA5807M_FUNC_BASS            0x1000
#define RDA5807M_FUNC_RCLKNONCAL      0x0800
#define RDA5807M_FUNC_RCLKDIRECT      0x0400
#define RDA5807M_FUNC_SEEKUP          0x0200
#define RDA5807M_FUNC_SEEK            0x0100
#define RDA5807M_FUNC_SKMODE          0x0080

#define RDA5807M_FUNC_CLKMODE_32kHz   0x0000
#define RDA5807M_FUNC_CLKMODE_12MHz   0x0010
#define RDA5807M_FUNC_CLKMODE_24MHz   0x0050
#define RDA5807M_FUNC_CLKMODE_13MHz   0x0020
#define RDA5807M_FUNC_CLKMODE_26MHz   0x0060
#define RDA5807M_FUNC_CLKMODE_19MHz   0x0030
#define RDA5807M_FUNC_CLKMODE_38MHz   0x0070

#define RDA5807M_FUNC_RDSEN           0x0008
#define RDA5807M_FUNC_NEWMETHOD       0x0000
#define RDA5807M_FUNC_SOFTRESET       0x0002
#define RDA5807M_FUNC_ENABLE          0x0001

// 0x03H Functions (Bit Enable)
#define RDA5807M_FUNC_CHANNEL        0x13C0
#define RDA5807M_FUNC_DIRECT         0x0020
#define RDA5807M_FUNC_TUNE           0x0010
#define RDA5807M_FUNC_BAND_0         0x0000
#define RDA5807M_FUNC_BAND_1         0x0004
#define RDA5807M_FUNC_BAND_2         0x0008
#define RDA5807M_FUNC_BAND_3         0x000C
#define RDA5807M_FUNC_SPACE_100kHz   0x0000
#define RDA5807M_FUNC_SPACE_200kHz   0x0001
#define RDA5807M_FUNC_SPACE_50kHz    0x0002
#define RDA5807M_FUNC_SPACE_25kHz    0x0003

// 0x04H Functions (Bit Enable)
#define RDA5807M_FUNC_RSVD_0         0x0000
#define RDA5807M_FUNC_STCIEN         0x4000
#define RDA5807M_FUNC_RBDS           0x2000
#define RDA5807M_FUNC_RDS_FIFO_EN    0x1000
#define RDA5807M_FUNC_DE             0x0800
#define RDA5807M_FUNC_RDS_FIFO_CLR   0x0400
#define RDA5807M_FUNC_SOFTMUTE_EN    0x0200
#define RDA5807M_FUNC_AFCD           0x0100
#define RDA5807M_FUNC_rsvd           0x0000
#define RDA5807M_FUNC_I2S_ENABLE     0x0040
#define RDA5807M_FUNC_GPIO3_HIGH_IMPEDANCE         0x0000
#define RDA5807M_FUNC_GPIO3_MONO_STEREO_INDICATOR  0x0010
#define RDA5807M_FUNC_GPIO3_LOW                    0x0020
#define RDA5807M_FUNC_GPIO3_HIGH                   0x0030
#define RDA5807M_FUNC_GPIO2_HIGH_IMPEDANCE         0x0000
#define RDA5807M_FUNC_GPIO2_INTERRUPT              0x0004
#define RDA5807M_FUNC_GPIO2_LOW                    0x0008
#define RDA5807M_FUNC_GPIO2_HIGH                   0x000C
#define RDA5807M_FUNC_GPIO1_HIGH_IMPEDANCE         0x0000
#define RDA5807M_FUNC_GPIO1_RESERVED               0x0001
#define RDA5807M_FUNC_GPIO1_LOW                    0x0002
#define RDA5807M_FUNC_GPIO1_HIGH                   0x0003

// 0x05H Functions (Bit Enable)
#define RDA5807M_FUNC_INT_MODE           0x8000
#define RDA5807M_FUNC_SEEK_MODE          0x4000
#define RDA5807M_FUNC_RSVD_1             0x0000
#define RDA5807M_FUNC_SEEKTH             0x0800
#define RDA5807M_FUNC_LNA_PORT_SEL_0     0x0000
#define RDA5807M_FUNC_LNA_PORT_SEL_1     0x0040
#define RDA5807M_FUNC_LNA_PORT_SEL_2     0x0080
#define RDA5807M_FUNC_LNA_PORT_SEL_3     0x00C0
#define RDA5807M_FUNC_LNA_ICSEL_BIT_0    0x0000
#define RDA5807M_FUNC_LNA_ICSEL_BIT_1    0x0010
#define RDA5807M_FUNC_LNA_ICSEL_BIT_2    0x0020
#define RDA5807M_FUNC_LNA_ICSEL_BIT_3    0x0030
#define RDA5807M_FUNC_VOLUME             0x000F

// 0x06H Functions (Bit Enable)
#define RDA5807M_FUNC_RSVD_2             0x0000
#define RDA5807M_FUNC_OPEN_MODE          0x5000
#define RDA5807M_FUNC_SLAVE_MASTER       0x1000
#define RDA5807M_FUNC_WS_LR              0x0800
#define RDA5807M_FUNC_SCLK_I_EDGE        0x0400
#define RDA5807M_FUNC_DATA_SIGNED        0x0200
#define RDA5807M_FUNC_WS_I_EDGE          0x0100
#define RDA5807M_FUNC_I2S_SW_CNT_48      0x0080
#define RDA5807M_FUNC_I2S_SW_CNT_44      0x0070
#define RDA5807M_FUNC_I2S_SW_CNT_32      0x0060
#define RDA5807M_FUNC_I2S_SW_CNT_24      0x0050
#define RDA5807M_FUNC_I2S_SW_CNT_22      0x0040
#define RDA5807M_FUNC_I2S_SW_CNT_16      0x0030
#define RDA5807M_FUNC_I2S_SW_CNT_12      0x0020
#define RDA5807M_FUNC_I2S_SW_CNT_11      0x0010
#define RDA5807M_FUNC_I2S_SW_CNT_8       0x0000
#define RDA5807M_FUNC_SW_O_EDGE          0x0008
#define RDA5807M_FUNC_SCLK_O_EDGE        0x0004
#define RDA5807M_FUNC_L_DELY             0x0002
#define RDA5807M_FUNC_R_DELY             0x0001

// 0x07H Functions (Bit Enable)
#define RDA5807M_FUNC_RSVD_3             0x0000
#define RDA5807M_FUNC_TH_SOFRBLEND       0x4000
#define RDA5807M_FUNC_65M_50M_MODE       0x0200
#define RDA5807M_FUNC_RSVD_4             0x0000
#define RDA5807M_FUNC_SEEK_TH_OLD        0x0400
#define RDA5807M_FUNC_SOFTBLEND_EN       0x0002
#define RDA5807M_FUNC_FREQ_MODE          0x0001

// 0x08H Functions
#define RDA5807M_FUNC_FREQ_DIRECT        0x0000

#define DISABLE 0
#define ENABLE  1

#define VOLUME_MIN 0
#define VOLUME_MAX 15
#define FREQ       88.0

typedef struct FmRadio {
  uint16_t chipid;
  uint16_t config;
  uint16_t tune;
  uint16_t gpio;
  uint16_t volume;
  uint16_t i2s;
  uint16_t blend;
  uint16_t freq;
} FmRadio;

int8_t init_rda5807();
int8_t write8(uint8_t addr, uint8_t data);
int8_t write16(uint8_t addr, uint16_t data);
int8_t writeL(const uint8_t *data, uint8_t len);

// 0x02H Functions
void dhiz(uint8_t flag);
void dmute(uint8_t flag);
void mono(uint8_t flag);
void bass(uint8_t flag);
void rclknoncal(uint8_t flag);
void rclkdirect(uint8_t flag);
void seekup(uint8_t flag);
void seek(uint8_t flag);
void skmode(uint8_t flag);
void clkmode(uint8_t clk);
void rdsen(uint8_t flag);
void softreset(uint8_t flag);
void enable(uint8_t flag);

// 0x03H Functions
void channel(float freq);
void tune(uint8_t flag);
void band(uint8_t b);
void space(uint8_t s);

// 0x05H Functions
void volume(uint16_t level);

extern FmRadio RDA5807;

#endif /* RDA5807M */