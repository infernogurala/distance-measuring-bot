#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* label = "INFERNO";
int16_t textW, textH;
int centerX, centerY;

int phase = 0;
unsigned long phaseStart = 0;

// Timing constants for display animation sequence
const int blackHoldStart = 600;   // hold on black before animation
const int lineGrowDur    = 900;   // line grows slowly from center
const int lineHoldDur    = 300;   // brief hold once line is full width
const int textFadeDur    = 700;   // text dissolves in
const int textHoldDur    = 1800;  // full hold duration to read text
const int blackHoldEnd   = 700;   // hold on black before loop repeats

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
    case 0: { // Hold on black
      if (elapsed >= blackHoldStart) { phase = 1; phaseStart = millis(); break; }
      display.clearDisplay();
      display.display();
      break;
    }

    case 1: { // Line grows slowly from center outward
      float t = (float)elapsed / lineGrowDur;
      if (t >= 1.0) { phase = 2; phaseStart = millis(); break; }
      float eased = 1.0 - pow(1.0 - t, 3); // ease-out cubic
      int lineHalfWidth = (int)(eased * (textW / 2 + 8));
      int lineY = centerY + textH + 10;

      display.clearDisplay();
      display.drawFastHLine(SCREEN_WIDTH / 2 - lineHalfWidth, lineY, lineHalfWidth * 2, SSD1306_WHITE);
      display.display();
      break;
    }

    case 2: { // Line holds at full width
      if (elapsed >= lineHoldDur) { phase = 3; phaseStart = millis(); break; }
      int lineY = centerY + textH + 10;
      display.clearDisplay();
      display.drawFastHLine(centerX - 8, lineY, textW + 16, SSD1306_WHITE);
      display.display();
      break;
    }

    case 3: { // Text dissolves IN while line stays
      float t = (float)elapsed / textFadeDur;
      if (t >= 1.0) { phase = 4; phaseStart = millis(); break; }

      display.clearDisplay();
      int lineY = centerY + textH + 10;
      display.drawFastHLine(centerX - 8, lineY, textW + 16, SSD1306_WHITE);

      display.setTextSize(2);
      display.setCursor(centerX, centerY);
      display.println(label);

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

    case 4: { // Full hold — text and line steady
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
