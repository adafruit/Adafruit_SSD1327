/*********************************************************************
This is a library for our grayscale OLEDs based on SSD1322 drivers

  Pick one up today in the adafruit shop!
  ------> https://www.adafruit.com/products/4741

These displays use I2C or SPI to communicate

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen below must be included in any
redistribution
*********************************************************************/

#include "Adafruit_SSD1322.h"

// CONSTRUCTORS, DESTRUCTOR ------------------------------------------------

/*!
    @brief  Constructor for I2C-interfaced SSD1322 displays.
    @param  w
            Display width in pixels
    @param  h
            Display height in pixels
    @param  twi
            Pointer to an existing TwoWire instance (e.g. &Wire, the
            microcontroller's primary I2C bus).
    @param  rst_pin
            Reset pin (using Arduino pin numbering), or -1 if not used
            (some displays might be wired to share the microcontroller's
            reset pin).
    @param  clkDuring
            Speed (in Hz) for Wire transmissions in SSD1322 library calls.
            Defaults to 400000 (400 KHz), a known 'safe' value for most
            microcontrollers, and meets the SSD1322 datasheet spec.
            Some systems can operate I2C faster (800 KHz for ESP32, 1 MHz
            for many other 32-bit MCUs), and some (perhaps not all)
            SSD1322's can work with this -- so it's optionally be specified
            here and is not a default behavior. (Ignored if using pre-1.5.7
            Arduino software, which operates I2C at a fixed 100 KHz.)
    @param  clkAfter
            Speed (in Hz) for Wire transmissions following SSD1322 library
            calls. Defaults to 100000 (100 KHz), the default Arduino Wire
            speed. This is done rather than leaving it at the 'during' speed
            because other devices on the I2C bus might not be compatible
            with the faster rate. (Ignored if using pre-1.5.7 Arduino
            software, which operates I2C at a fixed 100 KHz.)
    @note   Call the object's begin() function before use -- buffer
            allocation is performed there!
*/
Adafruit_SSD1322::Adafruit_SSD1322(uint16_t w, uint16_t h, TwoWire *twi,
                                   int8_t rst_pin, uint32_t clkDuring,
                                   uint32_t clkAfter)
    : Adafruit_GrayOLED(4, w, h, twi, rst_pin, clkDuring, clkAfter) {}

/*!
    @brief  Constructor for SPI SSD1322 displays, using software (bitbang)
            SPI.
    @param  w
            Display width in pixels
    @param  h
            Display height in pixels
    @param  mosi_pin
            MOSI (master out, slave in) pin (using Arduino pin numbering).
            This transfers serial data from microcontroller to display.
    @param  sclk_pin
            SCLK (serial clock) pin (using Arduino pin numbering).
            This clocks each bit from MOSI.
    @param  dc_pin
            Data/command pin (using Arduino pin numbering), selects whether
            display is receiving commands (low) or data (high).
    @param  rst_pin
            Reset pin (using Arduino pin numbering), or -1 if not used
            (some displays might be wired to share the microcontroller's
            reset pin).
    @param  cs_pin
            Chip-select pin (using Arduino pin numbering) for sharing the
            bus with other devices. Active low.
    @note   Call the object's begin() function before use -- buffer
            allocation is performed there!
*/
Adafruit_SSD1322::Adafruit_SSD1322(uint16_t w, uint16_t h, int8_t mosi_pin,
                                   int8_t sclk_pin, int8_t dc_pin,
                                   int8_t rst_pin, int8_t cs_pin)
    : Adafruit_GrayOLED(4, w, h, mosi_pin, sclk_pin, dc_pin, rst_pin, cs_pin) {}

/*!
    @brief  Constructor for SPI SSD1322 displays, using native hardware SPI.
    @param  w
            Display width in pixels
    @param  h
            Display height in pixels
    @param  spi
            Pointer to an existing SPIClass instance (e.g. &SPI, the
            microcontroller's primary SPI bus).
    @param  dc_pin
            Data/command pin (using Arduino pin numbering), selects whether
            display is receiving commands (low) or data (high).
    @param  rst_pin
            Reset pin (using Arduino pin numbering), or -1 if not used
            (some displays might be wired to share the microcontroller's
            reset pin).
    @param  cs_pin
            Chip-select pin (using Arduino pin numbering) for sharing the
            bus with other devices. Active low.
    @param  bitrate
            SPI clock rate for transfers to this display. Default if
            unspecified is 8000000UL (8 MHz).
    @note   Call the object's begin() function before use -- buffer
            allocation is performed there!
*/
Adafruit_SSD1322::Adafruit_SSD1322(uint16_t w, uint16_t h, SPIClass *spi,
                                   int8_t dc_pin, int8_t rst_pin, int8_t cs_pin,
                                   uint32_t bitrate)
    : Adafruit_GrayOLED(4, w, h, spi, dc_pin, rst_pin, cs_pin, bitrate) {}

/*!
    @brief  Destructor for Adafruit_SSD1322 object.
*/
Adafruit_SSD1322::~Adafruit_SSD1322(void) {}

// ALLOCATE & INIT DISPLAY -------------------------------------------------

/*!
    @brief  Allocate RAM for image buffer, initialize peripherals and pins.
    @param  addr
            I2C address of corresponding SSD1322 display.
            SPI displays (hardware or software) do not use addresses, but
            this argument is still required (pass 0 or any value really,
            it will simply be ignored). Default if unspecified is 0.
    @param  reset
            If true, and if the reset pin passed to the constructor is
            valid, a hard reset will be performed before initializing the
            display. If using multiple SSD1322 displays on the same bus, and
            if they all share the same reset pin, you should only pass true
            on the first display being initialized, false on all others,
            else the already-initialized displays would be reset. Default if
            unspecified is true.
    @return true on successful allocation/init, false otherwise.
            Well-behaved code should check the return value before
            proceeding.
    @note   MUST call this function before any drawing or updates!
*/
bool Adafruit_SSD1322::begin(uint8_t addr, bool reset) {

  if (!Adafruit_GrayOLED::_init(addr, reset)) {
    return false;
  }

  static const uint8_t init_256x64[] {
    2, 0xfd, 0x12,            	        /* unlock */
    1, 0xae,		                /* display off */
    2, 0xb3, 0x91,			/* set display clock divide ratio/oscillator frequency (set clock as 80 frames/sec)  */  
    2, 0xca, 0x3f,			/* multiplex ratio 1/64 Duty (0x0F~0x3F) */  
    2, 0xa2, 0x00,			/* display offset, shift mapping ram counter */  
    2, 0xa1, 0x00,			/* display start line */  
    3, 0xa0, 0x06, 0x11,	        /* Set Re-Map / Dual COM Line Mode */
    2, 0xab, 0x01,			/* Enable Internal VDD Regulator */  
    3, 0xb4, 0xa0, 0x05|0xfd,	        /* Display Enhancement A */  
    2, 0xc1, 0x9f,			/* contrast */  
    2, 0xc7, 0x0f,			/* Set Scale Factor of Segment Output Current Control */  
    1, 0xb9,		                /* linear grayscale */
    2, 0xb1, 0xe2,			/* Phase 1 (Reset) & Phase 2 (Pre-Charge) Period Adjustment */  
    3, 0xd1, 0x82|0x20, 0x20,	        /* Display Enhancement B */  
    2, 0xbb, 0x1f,			/* precharge  voltage */  
    2, 0xb6, 0x08,			/* precharge  period */  
    2, 0xbe, 0x07,			/* vcomh */  
    1, 0xa6,		                /* normal display */
    1, 0xa9		                /* exit partial display */
  };
  
  page_offset = 0;
  if (!oled_commandAndArgsList(init_256x64, sizeof(init_256x64))) {
    return false;
  }

  delay(2);           // 2ms delay recommended
  oled_command(0xaf); // 0xaf

  memset(buffer, 0x00, _bpp * WIDTH * ((HEIGHT + 7) / 8));

  return true; // Success
}

/*!
    @brief  Do the actual writing of the internal frame buffer to display RAM
*/
void Adafruit_SSD1322::display(void) {
  // ESP8266 needs a periodic yield() call to avoid watchdog reset.
  // With the limited size of SSD1327 displays, and the fast bitrate
  // being used (1 MHz or more), I think one yield() immediately before
  // a screen write and one immediately after should cover it.  But if
  // not, if this becomes a problem, yields() might be added in the
  // 32-byte transfer condition below.
  yield();

  uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
  uint8_t *ptr = buffer;
  uint8_t dc_byte = 0x40;

  uint8_t bytes_per_row = WIDTH / 2; // See fig 10-1 (64 bytes, 128 pixels)
  uint8_t maxbuff = 128;

//   Serial.print("Window: (");
//   Serial.print(window_x1);
//   Serial.print(", ");
//   Serial.print(window_y1);
//   Serial.print(") -> (");
//   Serial.print(window_x2);
//   Serial.print(", ");
//   Serial.print(window_y2);
//   Serial.println(")");
  
  uint16_t x_start = min(WIDTH-1, window_x1);
  uint16_t x_end = max(0, window_x2);
  uint8_t col_start = (uint8_t)(x_start/4);
  uint8_t col_end = (uint8_t)(x_end/4);

  uint8_t row_start = (uint8_t) min((int16_t)(HEIGHT-1), (int16_t)window_y1);
  uint8_t row_end = (uint8_t) max((int16_t)0, (int16_t)window_y2);

//   Serial.print("row start/end: ");
//   Serial.print(row_start);
//   Serial.print(" -> ");
//   Serial.println(row_end);
//   Serial.print("x start/end: ");
//   Serial.print(x_start);
//   Serial.print(" -> ");
//   Serial.println(x_end);
//   Serial.print("col start/end: ");
//   Serial.print(col_start);
//   Serial.print(" -> ");
//   Serial.println(col_end);

  if (i2c_dev) { // I2C
    // Set high speed clk
    i2c_dev->setSpeed(i2c_preclk);
    maxbuff = i2c_dev->maxBufferSize() - 1;
  }

  uint8_t cmd[] = {
          3, 0x75, row_start, row_end,
          3, 0x15, (uint8_t)(28+col_start), (uint8_t)(28+col_end)  // Add OLED panel offset
  };

  oled_commandAndArgsList(cmd, sizeof(cmd));
  oled_command(0x5c);

  for (uint8_t row = row_start; row <= row_end; row++) {
    uint8_t bytes_remaining = (col_end-col_start)*2+2;
    ptr = buffer + (uint16_t)row * (uint16_t)bytes_per_row;
    // fast forward to dirty rectangle beginning
    ptr += col_start*2;

    while (bytes_remaining) {
      uint8_t to_write = min(bytes_remaining, maxbuff);
      if (i2c_dev) {
        i2c_dev->write(ptr, to_write, true, &dc_byte, 1);
      } else {
        digitalWrite(dcPin, HIGH);
        spi_dev->write(ptr, to_write);
      }
      ptr += to_write;
      bytes_remaining -= to_write;
      yield();
    }
  }

  if (i2c_dev) { // I2C
    // Set low speed clk
    i2c_dev->setSpeed(i2c_postclk);
  }

  // reset dirty window
  window_x1 = 1024;
  window_y1 = 1024;
  window_x2 = -1;
  window_y2 = -1;
}

/*!
    @brief  Enable or disable display invert mode (white-on-black vs
            black-on-white). Handy for testing!
    @param  i
            If true, switch to invert mode (black-on-white), else normal
            mode (white-on-black).
    @note   This has an immediate effect on the display, no need to call the
            display() function -- buffer contents are not changed, rather a
            different pixel mode of the display hardware is used. When
            enabled, drawing MONOOLED_BLACK (value 0) pixels will actually draw
   white, MONOOLED_WHITE (value 1) will draw black.
*/

void Adafruit_SSD1322::invertDisplay(bool i) {
  oled_command(i ? 0xa7: 0xa6);
}
