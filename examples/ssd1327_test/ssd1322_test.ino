#include <SPI.h>
#include <Adafruit_SSD1322.h>

// Used for software SPI
#define OLED_CLK 13
#define OLED_MOSI 11

// Used for software or hardware SPI
#define OLED_CS 10
#define OLED_DC 9

// Used for I2C or SPI
#define OLED_RESET 8
#define OLED_ROTATION 2  // CHANGEME to 0 if your panel is mounted differently

// software SPI
//Adafruit_SSD1305 display(256, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
// hardware SPI
Adafruit_SSD1322 display(256, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS, 1000000);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };


void setup()   {                
  Serial.begin(115200);
  //while (! Serial) delay(100);
  Serial.println("SSD1322 OLED test");
  
  if ( ! display.begin(0x3D) ) {
     Serial.println("Unable to initialize OLED");
     while (1) yield();
  }

  // Note: change me if your panel orientation is different than mine
  display.setRotation(OLED_ROTATION);

  Serial.println("SSD1322 clear");
  display.clearDisplay();
  display.display();

  // draw rectangles
  Serial.println("SSD1322 draw rect");
  testdrawrect();
  display.display();
  delay(1000);
  display.clearDisplay();

  // draw a SSD1322_WHITE circle, 10 pixel radius
  Serial.println("SSD1322 fillCircle");
  display.fillCircle(display.width()/2, display.height()/2, 10, SSD1322_WHITE);
  display.display();
  delay(1000);
  display.clearDisplay();

  // draw the first ~12 characters in the font
  Serial.println("SSD1322 testdrawchar");
  testdrawchar();
  display.display();
  delay(1000);
  display.clearDisplay();

  // draw multiple rectangles
  testfillrect();
  display.display();
  delay(1000);
  display.clearDisplay();

  // draw mulitple circles
  testdrawcircle();
  display.display();
  delay(1000);
  display.clearDisplay();

  testdrawroundrect();
  delay(1000);
  display.clearDisplay();

  testfillroundrect();
  delay(1000);
  display.clearDisplay();

  testdrawtriangle();
  delay(1000);
  display.clearDisplay();
   
  testfilltriangle();
  delay(1000);
  display.clearDisplay();

  for (uint8_t rot=0; rot < 4; rot++) {
    display.setRotation(rot);
    display.clearDisplay();
    // text display tests
    display.setTextSize(1);
    display.setTextColor(SSD1322_WHITE);
    display.setCursor(0,0);
    display.println("Hello, world!");
    display.setTextColor(SSD1322_BLACK, SSD1322_WHITE); // 'inverted' text
    display.println(3.141592);
    display.setTextSize(2);
    display.setTextColor(SSD1322_WHITE);
    display.print("0x"); display.println(0xDEADBEEF, HEX);
    display.display();
    delay(1000);
  }

  display.setRotation(OLED_ROTATION);
  
  // miniature bitmap display
  display.clearDisplay();
  display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  display.display();

  // invert the display
  display.invertDisplay(true);
  delay(1000); 
  display.invertDisplay(false);
  delay(1000); 

  // draw a bitmap icon and 'animate' movement
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}


void loop() {
}


void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];
  randomSeed(666);     // whatever seed
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
    
    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  while (1) {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, f+1);
    }
    display.display();
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, SSD1322_BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
      	icons[f][XPOS] = random(display.width());
      	icons[f][YPOS] = 0;
      	icons[f][DELTAY] = random(5) + 1;
      }
    }
   }
}


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextWrap(false);
  display.setTextColor(SSD1322_WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }    
  display.display();
}

void testdrawcircle(void) {
  for (uint8_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, i % 15 + 1);
    display.display();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (uint8_t i=0; i<display.height()/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2,  i % 15 + 1);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) {
  for (uint16_t i=0; i<min(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i,  i % 15 + 1);
    display.display();
  }
}

void testfilltriangle(void) {
  uint8_t color = SSD1322_WHITE;
  for (int16_t i=min(display.width(),display.height())/2; i>0; i-=5) {
    display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i,  i % 15 + 1);
    display.display();
  }
}

void testdrawroundrect(void) {
  for (uint8_t i=0; i<display.height()/3-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4,  i % 15 + 1);
    display.display();
  }
}

void testfillroundrect(void) {
  uint8_t color = SSD1322_WHITE;
  for (uint8_t i=0; i<display.height()/3-2; i+=2) {
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4,  i % 15 + 1);
    display.display();
  }
}
   
void testdrawrect(void) {
  for (uint8_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, i % 15 + 1);
    display.display();
  }
}