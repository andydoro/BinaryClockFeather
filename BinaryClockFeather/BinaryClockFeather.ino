// Date and time functions using an RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

// NeoMatrix libraries
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#if defined(ARDUINO_ARCH_SAMD)
  // for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
  #define Serial SerialUSB
#endif

#define BRIGHTNESS 40 // the brightness

RTC_PCF8523 rtc;

//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int sunit, sten, munit, mten, hunit, hten, dunit, dten;


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 4, 6,
                            NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
                            NEO_MATRIX_ROWS   + NEO_MATRIX_PROGRESSIVE,
                            NEO_GRB           + NEO_KHZ800);


const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255), matrix.Color(255, 255, 255)
};




void setup () {

  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  matrix.begin();
  matrix.setBrightness(BRIGHTNESS);

  matrix.show(); // Initialize all pixels to 'off'

  // startup test sequence
  for (int i = 0; i < 4; i++) {
    matrix.fillScreen(colors[i]);
    matrix.show();
    delay(500);
  }
}

void loop () {
  DateTime theTime = rtc.now();

  // reset matrix
  matrix.fillScreen(0);

  // break up time into different digits
  sunit = theTime.second() % 10;
  sten  = theTime.second() / 10;
  munit = theTime.minute() % 10;
  mten  = theTime.minute() / 10;
  hunit = theTime.hour() % 10;
  hten  = theTime.hour() / 10;
  dunit = theTime.day() % 10;
  dten  = theTime.day() / 10;

  //seconds second digit
  if (sunit % 2 == 1) {
    matrix.drawPixel(7, 0, colors[2]);
  }
  if (sunit == 2 || sunit == 3 || sunit == 6 || sunit == 7) {
    matrix.drawPixel(7, 1, colors[2]);
  }
  if (sunit == 4 || sunit == 5 || sunit == 6 || sunit == 7) {
    matrix.drawPixel(7, 2, colors[2]);
  }
  if (sunit == 8 || sunit == 9) {
    matrix.drawPixel(7, 3, colors[2]);
  }

  // seconds first digit
  if (sten % 2 == 1) {
    matrix.drawPixel(6, 0, colors[2]);
  }
  if (sten == 2 || sten == 3) {
    matrix.drawPixel(6, 1, colors[2]);
  }
  if (sten == 4 || sten == 5) {
    matrix.drawPixel(6, 2, colors[2]);
  }

  // minutes second digit
  if (munit % 2 == 1) {
    matrix.drawPixel(5, 0, colors[1]);
  }
  if (munit == 2 || munit == 3 || munit == 6 || munit == 7) {
    matrix.drawPixel(5, 1, colors[1]);
  }
  if (munit == 4 || munit == 5 || munit == 6 || munit == 7) {
    matrix.drawPixel(5, 2, colors[1]);
  }
  if (munit == 8 || munit == 9) {
    matrix.drawPixel(5, 3, colors[1]);
  }

  // minutes first digit
  if (mten % 2 == 1) {
    matrix.drawPixel(4, 0, colors[1]);
  }
  if (mten == 2 || mten == 3) {
    matrix.drawPixel(4, 1, colors[1]);
  }
  if (mten == 4 || mten == 5) {
    matrix.drawPixel(4, 2, colors[1]);
  }

  // hours second digit
  if (hunit % 2 == 1) {
    matrix.drawPixel(3, 0, colors[0]);
  }
  if (hunit == 2 || hunit == 3 || hunit == 6 || hunit == 7) {
    matrix.drawPixel(3, 1, colors[0]);
  }
  if (hunit == 4 || hunit == 5 || hunit == 6 || hunit == 7) {
    matrix.drawPixel(3, 2, colors[0]);
  }
  if (hunit == 8 || hunit == 9) {
    matrix.drawPixel(3, 3, colors[0]);
  }

  // hours first digit (24 hour time)
  if (hten == 1) {
    matrix.drawPixel(2, 0, colors[0]);
  }
  if (hten == 2) {
    matrix.drawPixel(2, 1, colors[0]);
  }

  // days second digit
  if (dunit % 2 == 1) {
    matrix.drawPixel(1, 0, colors[3]);
  }
  if (dunit == 2 || dunit == 3 || dunit == 6 || dunit == 7) {
    matrix.drawPixel(1, 1, colors[3]);
  }
  if (dunit == 4 || dunit == 5 || dunit == 6 || dunit == 7) {
    matrix.drawPixel(1, 2, colors[3]);
  }
  if (dunit == 8 || dunit == 9) {
    matrix.drawPixel(1, 3, colors[3]);
  }

  // days first digit
  if (dten % 2 == 1) {
    matrix.drawPixel(0, 0, colors[3]);
  }
  if (dten > 2) {
    matrix.drawPixel(0, 1, colors[3]);
  }

  Serial.print(theTime.year(), DEC);
  Serial.print('/');
  Serial.print(theTime.month(), DEC);
  Serial.print('/');
  Serial.print(theTime.day(), DEC);
  Serial.print(" (");
  //Serial.print(daysOfTheWeek[theTime.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(theTime.hour(), DEC);
  Serial.print(':');
  Serial.print(theTime.minute(), DEC);
  Serial.print(':');
  Serial.print(theTime.second(), DEC);
  Serial.println();

  matrix.show();
  delay(100);
}
