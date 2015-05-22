#include "Adafruit_NeoPixel.h"
#include <Wire.h>
#include <EEPROM.h>
#include <avr/power.h>
#define PIN 5
#define  LED_Number  8
#define  pi  3.14
#define  Resolution  100
#define  NumAdj      1.3
#define  AddBlack    8
#define  AddWhite    16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_Number, PIN, NEO_GRB + NEO_KHZ800);
int  ThresholdADC = 850;
byte  LineValue;
int  Line[8], CaliLine[8], LineOffset[8], LineBlack[8], LineWhite[8], LineThreshold[8];
int  SumLine, AveLine;
byte  i1, i2;
byte  AddressI2C;
float  rad;
byte  Rp, Gp, Bp;
byte  LineData;
int  Decentration;
int    DecentCaliData, DecentUntouchedData;
int   MinLine;
int  AveNonCali, AveCali;
int addr = 0;
char  GetSerialData;

void LookLineADC() {
  int RotationNum = 1;
  LineValue = 0;
  SumLine = 0;
  for (i1 = 0; i1 < 8; i1++) {
    Line[i1] = analogRead(i1);  // read the input pin
    if (Line[i1] > LineThreshold[i1] ) {
      LineValue = LineValue + RotationNum * 1;
      strip.setPixelColor(i1, 0, 50, 50);
    } else {
      strip.setPixelColor(i1, 0, 0, 0);
    }
    RotationNum = RotationNum << 1;
  }

  LineData = LineValue;

  strip.show();
}

void OffsetLine() {

  LookLineADC();
  CalcMin();
  for (i1 = 0; i1 < 8; i1++) {
    LineOffset[i1] = Line[i1] - MinLine;
  }
}

void setup()
{

#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  AddressI2C =  digitalRead(0) + 2 * digitalRead(1) + 16;

  Wire.begin(AddressI2C);                // join i2c bus with address #2
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);

  pinMode(4, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(13, OUTPUT);

  for (i1 = 0; i1 < 8; i1++) {
    LineBlack[i1] = EEPROM.read(i1 + AddBlack);
    LineWhite[i1] = EEPROM.read(i1 + AddWhite);
    LineThreshold[i1] = (LineBlack[i1] + LineWhite[i1] ) * 2;
  }
}
void loop()
{
  LookLineADC();
  AveCali = CalcAverage(CaliLine);
  AveNonCali = CalcAverage(Line);
  DecentCaliData = CalcDecentration(CaliLine, AveCali) ;
  DecentUntouchedData = CalcDecentration(Line, AveNonCali) ;
  Serial.print("I2C Add:");
  Serial.print(AddressI2C, DEC);

  Serial.print("  Untouched: ");
  for (i1 = 0; i1 < 8; i1++) {
    Serial.print(Line[i1], DEC);
    Serial.print(" ");
  }
  Serial.print("  Threshold: ");
  for (i1 = 0; i1 < 8; i1++) {
    Serial.print(LineThreshold[i1]);
    Serial.print(" ");
  }
  //  Serial.print("Average:");
  //  Serial.print(AveLine, DEC);
  Serial.print("  DecentNonCali: ");
  Serial.print(DecentUntouchedData);
  for (i1 = 0; i1 < 8; i1++) {
    if (Line[i1] > LineThreshold[i1]) {
      Serial.print(" O");
    } else {
      Serial.print(" _");
    }
  }
  Serial.println("");
  delayMicroseconds(500);

  while (Serial.available() > 0) {
    //  OffsetLine();
    GetSerialData = Serial.read();
    switch (GetSerialData) {
      case 's':  Serial.print(7, DEC);
        for (i1 = 0; i1 < 8; i1++) {
          EEPROM.write(AddBlack + i1, Line[i1] / 4);
        }
        break;

      case'b':    Serial.println("Black Calibration");
        for (i1 = 0; i1 < 8; i1++) {
          EEPROM.write(AddBlack + i1, Line[i1] / 4);
        }
        break;

      case'w':    Serial.println("White Calibration");
        for (i1 = 0; i1 < 8; i1++) {
          EEPROM.write(AddWhite + i1, Line[i1] / 4);
        }
        break;

      case 'o':  for (i1 = 0; i1 < 8; i1++) {
          LineBlack[i1] = EEPROM.read(i1 + AddBlack);
          LineWhite[i1] = EEPROM.read(i1 + AddWhite);
          LineThreshold[i1] = (LineBlack[i1]  + LineWhite[i1]) * 2;
          Serial.print(" LineWhite:");
          Serial.print(LineBlack[i1] * 4, DEC);
          Serial.print(" White:");
          Serial.print(LineWhite[i1] * 4, DEC);
          Serial.print(" Threshold:");
          Serial.println(LineThreshold[i1]  , DEC);
        }
        break;
    }
  }
}

int  CalcDecentration(int *Data, int Ave) {
  Decentration = 0;
  for (i1 = 0; i1 < 8; i1++) {
    Decentration = (Decentration + (Data[i1] - Ave) * (Data[i1] - Ave)) / 100;
  }
  return Decentration;
}
int  CalcAverage(int *Data) {
  for (i1 = 0; i1 < 8; i1++) {
    SumLine = SumLine + Data[i1];
    AveLine = SumLine / 8;
  }
  return AveLine;
}
void CalcMin() {
  MinLine = 1000;
  for (i1 = 0; i1 < 8; i1++) {
    if (MinLine > Line[i1]) {
      MinLine = Line[i1];
    }
  }

}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{
  Wire.write(LineData); // respond with message of 6 bytes
  //  Wire.write("hello "); // respond with message of 6 bytes
  // Wire.write(LineValue);

  //  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  //  delay(100);              // wait for a second
  //  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW

}
void receiveEvent(int howMany)
{
  while (1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c, DEC);        // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}
