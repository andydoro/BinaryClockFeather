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

int digits[8]; // array for each "digit"

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 4, 6,
                            NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
                            NEO_MATRIX_ROWS   + NEO_MATRIX_PROGRESSIVE,
                            NEO_GRB           + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(0, 0, 255), matrix.Color(0, 255, 0), matrix.Color(255, 0, 0), matrix.Color(255, 0, 255)
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
  digits[0] = theTime.second() % 10;
  digits[1] = theTime.second() / 10;
  digits[2] = theTime.minute() % 10;
  digits[3] = theTime.minute() / 10;
  digits[4] = theTime.hour() % 10;
  digits[5] = theTime.hour() / 10;
  digits[6] = theTime.day() % 10;
  digits[7] = theTime.day() / 10;

  // compute binary and draw
  for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 4; i++) {
      if (bitRead(digits[j], i) == 1) {
        matrix.drawPixel(7 - j, i, colors[j / 2]);
      }
    }
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
