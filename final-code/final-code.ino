#include <Wire.h>

#include <SoftwareSerial.h>

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

  

#define SCREEN_WIDTH 128

#define SCREEN_HEIGHT 64

#define OLED_RESET -1

#define SCREEN_ADDRESS 0x3C

  

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

SoftwareSerial BT(8, 9);           // RX = 8 (from BT TX), TX = 9 (to BT RX)

  

const int SPEED = 150;             // keep the same: formula was fitted at this speed

  

// Fitted from your tests at SPEED 150

const float V_CM_S = 71.2;         // steady speed

const float TAU_S  = 0.5;          // pickup lag

  

int in1 = 7;

int in2 = 2;

int enA = 3;

int in3 = 4;

int in4 = 5;

int enB = 6;

  

enum State { STOPPED, FWD, BACK, LEFT, RIGHT };

State state = STOPPED;

  

float totalCm = 0;                 // net distance from committed segments

unsigned long segStart = 0;

unsigned long lastUpdate = 0;

  

// Distance for a straight run of t seconds starting from rest

float distanceAt(float t) {

  if (t <= 0) return 0;

  return V_CM_S * (t - TAU_S * (1.0 - exp(-t / TAU_S)));

}

  

// Live net distance (committed + current segment)

float currentCm() {

  float t = (millis() - segStart) / 1000.0;

  if (state == FWD)  return totalCm + distanceAt(t);

  if (state == BACK) return totalCm - distanceAt(t);

  return totalCm;

}

  

void showDistance(float cm, const char* status) {

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(0, 0);

  display.print(status);

  display.setTextSize(3);

  display.setCursor(0, 25);

  display.print(cm, 0);

  display.setTextSize(2);

  display.print(" cm");

  display.display();

}

  

void setMotors(bool a1, bool a2, bool b1, bool b2, int pwm) {

  analogWrite(enA, pwm);

  analogWrite(enB, pwm);

  digitalWrite(in1, a1);

  digitalWrite(in2, a2);

  digitalWrite(in3, b1);

  digitalWrite(in4, b2);

}

  

// Close the current segment and add its distance to the total

void commitSegment() {

  totalCm = currentCm();

}

  

void changeState(State s) {

  if (s == state) return;

  commitSegment();

  state = s;

  segStart = millis();

  

  switch (s) {

    case FWD:     setMotors(HIGH, LOW,  HIGH, LOW,  SPEED); break;

    case BACK:    setMotors(LOW,  HIGH, LOW,  HIGH, SPEED); break;

    case LEFT:    setMotors(LOW,  HIGH, HIGH, LOW,  SPEED); break;  // swap with RIGHT if reversed

    case RIGHT:   setMotors(HIGH, LOW,  LOW,  HIGH, SPEED); break;

    case STOPPED: setMotors(LOW,  LOW,  LOW,  LOW,  0);     break;

  }

  sendDistance();

}

  

void sendDistance() {

  BT.print("D:");

  BT.print(currentCm(), 0);

  BT.println(" cm");

}

  

void handleCommand(char c) {

  switch (toupper(c)) {

    case 'F': changeState(FWD);     break;

    case 'B': changeState(BACK);    break;

    case 'L': changeState(LEFT);    break;

    case 'R': changeState(RIGHT);   break;

    case 'S': changeState(STOPPED); break;

    case 'Y': changeState(STOPPED); totalCm = 0; sendDistance(); break;

  }

}

  

void setup() {

  Serial.begin(9600);

  BT.begin(9600);                  // HC-05/HC-06 default baud

  

  pinMode(in1, OUTPUT);

  pinMode(in2, OUTPUT);

  pinMode(in3, OUTPUT);

  pinMode(in4, OUTPUT);

  pinMode(enA, OUTPUT);

  pinMode(enB, OUTPUT);

  

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {

    Serial.println(F("SSD1306 allocation failed"));

  }

  showDistance(0, "Ready");

  setMotors(LOW, LOW, LOW, LOW, 0);

}

  

void loop() {

  while (BT.available()) {

    char c = BT.read();

    if (c != '\r' && c != '\n') handleCommand(c);

  }

  

  unsigned long now = millis();

  if (now - lastUpdate >= 100) {

    lastUpdate = now;

    const char* label = (state == STOPPED) ? "Stopped" :

                        (state == FWD)     ? "Forward" :

                        (state == BACK)    ? "Backward" : "Turning";

    showDistance(currentCm(), label);

    if (state == FWD || state == BACK) {

      static unsigned long lastBT = 0;

      if (now - lastBT >= 500) { lastBT = now; sendDistance(); }

    }

  }

}