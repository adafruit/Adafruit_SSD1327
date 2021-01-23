/*********************************************************************
This is a library for our Grayscale OLEDs based on SSD1322 drivers

  Pick one up today in the adafruit shop!
  ------> https://www.adafruit.com/products/4741

These displays use I2C or SPI to communicate

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <Adafruit_GrayOLED.h>

#define SSD1322_BLACK 0x0
#define SSD1322_WHITE 0xF

#define SSD1322_I2C_ADDRESS 0x3D

/*! The controller object for SSD1322 OLED displays */
class Adafruit_SSD1322 : public Adafruit_GrayOLED {
public:
  Adafruit_SSD1322(uint16_t w, uint16_t h, TwoWire *twi = &Wire,
                   int8_t rst_pin = -1, uint32_t preclk = 400000,
                   uint32_t postclk = 100000);
  Adafruit_SSD1322(uint16_t w, uint16_t h, int8_t mosi_pin, int8_t sclk_pin,
                   int8_t dc_pin, int8_t rst_pin, int8_t cs_pin);
  Adafruit_SSD1322(uint16_t w, uint16_t h, SPIClass *spi, int8_t dc_pin,
                   int8_t rst_pin, int8_t cs_pin, uint32_t bitrate = 8000000UL);
  ~Adafruit_SSD1322(void);

  bool begin(uint8_t i2caddr = SSD1322_I2C_ADDRESS, bool reset = true);
  void display();
  void invertDisplay(bool i);

  void jh_fill(uint8_t p);

private:
  int8_t page_offset = 0;
  int8_t column_offset = 0;
};
