```ino
#include <Wire.h>

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

  

#define SCREEN_WIDTH 128

#define SCREEN_HEIGHT 64

#define OLED_RESET    -1

#define SCREEN_ADDRESS 0x3C

  

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  

const char* label = "INFERNO";

int16_t textW, textH;

int centerX, centerY;

  

int phase = 0;

unsigned long phaseStart = 0;

  

// Slower, more generous timing — enough for each beat to actually register

const int blackHoldStart = 600;   // hold on black before anything happens

const int lineGrowDur    = 900;   // line grows slowly, deliberately

const int lineHoldDur    = 300;   // brief hold once line is full width

const int textFadeDur    = 700;   // text dissolves IN (reverse of your dissolve-out)

const int textHoldDur    = 1800;  // long hold so it can actually be read

const int blackHoldEnd   = 700;   // hold on black before loop

  

void setup() {

  Serial.begin(9600);

  randomSeed(analogRead(A0));

  

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {

    Serial.println(F("SSD1306 allocation failed"));

    while (true);

  }

  

  display.setTextSize(2);

  display.setTextColor(SSD1306_WHITE);

  

  int16_t x1, y1;

  uint16_t w, h;

  display.getTextBounds(label, 0, 0, &x1, &y1, &w, &h);

  textW = w;

  textH = h;

  centerX = (SCREEN_WIDTH - textW) / 2;

  centerY = (SCREEN_HEIGHT - textH) / 2 - 3;

  

  phaseStart = millis();

}

  

void loop() {

  unsigned long elapsed = millis() - phaseStart;

  

  switch (phase) {

  

    case 0: { // Just hold on black — let it breathe first

      if (elapsed >= blackHoldStart) { phase = 1; phaseStart = millis(); break; }

      display.clearDisplay();

      display.display();

      break;

    }

  

    case 1: { // Line grows slowly from center outward

      float t = (float)elapsed / lineGrowDur;

      if (t >= 1.0) { phase = 2; phaseStart = millis(); break; }

      // smooth ease, no snapping

      float eased = 1.0 - pow(1.0 - t, 3); // ease-out cubic

      int lineHalfWidth = (int)(eased * (textW / 2 + 8));

      int lineY = centerY + textH + 10;

  

      display.clearDisplay();

      display.drawFastHLine(SCREEN_WIDTH / 2 - lineHalfWidth, lineY, lineHalfWidth * 2, SSD1306_WHITE);

      display.display();

      break;

    }

  

    case 2: { // Line holds at full width, alone, briefly

      if (elapsed >= lineHoldDur) { phase = 3; phaseStart = millis(); break; }

      int lineY = centerY + textH + 10;

      display.clearDisplay();

      display.drawFastHLine(centerX - 8, lineY, textW + 16, SSD1306_WHITE);

      display.display();

      break;

    }

  

    case 3: { // Text dissolves IN (pixels build up gradually) while line stays

      float t = (float)elapsed / textFadeDur;

      if (t >= 1.0) { phase = 4; phaseStart = millis(); break; }

  

      display.clearDisplay();

  

      // draw full text to a decision, then only plot a fraction of its pixels

      int lineY = centerY + textH + 10;

      display.drawFastHLine(centerX - 8, lineY, textW + 16, SSD1306_WHITE);

  

      // build up text pixel-by-pixel using a mask approach:

      // draw at full opacity but mask random pixels OFF, inverse of dissolve-out

      display.setTextSize(2);

      display.setCursor(centerX, centerY);

      display.println(label);

  

      // now erase pixels that shouldn't be visible yet (reverse dissolve)

      int totalPixels = textW * textH;

      int pixelsStillHidden = (int)(totalPixels * (1.0 - t) * 1.3);

      for (int i = 0; i < pixelsStillHidden; i++) {

        int px = centerX + random(0, textW);

        int py = centerY + random(0, textH);

        display.drawPixel(px, py, SSD1306_BLACK);

      }

  

      display.display();

      break;

    }

  

    case 4: { // Full hold — text and line steady, nothing moving, long enough to read

      if (elapsed >= textHoldDur) { phase = 5; phaseStart = millis(); break; }

      display.clearDisplay();

      display.setTextSize(2);

      display.setCursor(centerX, centerY);

      display.println(label);

      int lineY = centerY + textH + 10;

      display.drawFastHLine(centerX - 8, lineY, textW + 16, SSD1306_WHITE);

      display.display();

      break;

    }

  

    case 5: { // Hold on black before looping

      if (elapsed >= blackHoldEnd) { phase = 0; phaseStart = millis(); break; }

      display.clearDisplay();

      display.display();

      break;

    }

  }

  

  delay(16);

}
```