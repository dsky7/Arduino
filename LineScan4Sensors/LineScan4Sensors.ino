#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define DelayClock  20
#define PIN            6
#define NUMPIXELS      8
#define DEBUG
#define  maskFR    20
#define  maskRL    40
const int analogInPin1 = A0, analogInPin2 = A1, analogInPin3 = A8, analogInPin4 = A10;
const int CLK = 4;
const int SI = 11;
int  threshold=100,thresholdLOW,thresholdHIGH;
byte Adata;
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int AnalogValue1[128], AnalogValue2[128], AnalogValue3[128], AnalogValue4[128];
int i1;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pinMode(CLK, OUTPUT);
  pinMode(SI, OUTPUT);
  digitalWrite(CLK, LOW);

  pixels.begin();
  for (int i = 0; i < NUMPIXELS; i++) {
    if (i < (NUMPIXELS / 2)) {
      pixels.setPixelColor(i, pixels.Color(200 * (4 - i) / 4, 200, 200));
    } else {
      pixels.setPixelColor(i, pixels.Color(200 - (NUMPIXELS - 1 - i) * 25, 0, 200));
    }
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
}

void GetData(void) {
  digitalWrite(SI, HIGH);
  delayMicroseconds(DelayClock);

  for (i1 = 0; i1 < 128; i1++) {
    digitalWrite(CLK, HIGH);
    delayMicroseconds(DelayClock);

    digitalWrite(SI, LOW);

    AnalogValue1[i1] = analogRead(analogInPin1);
    AnalogValue2[i1] = analogRead(analogInPin2);
    AnalogValue3[i1] = analogRead(analogInPin3);
    AnalogValue4[i1] = analogRead(analogInPin4);
    digitalWrite(CLK, LOW);
    delayMicroseconds(DelayClock);

  }
  digitalWrite(CLK, HIGH);
  delayMicroseconds(DelayClock);
  digitalWrite(CLK, LOW);
  delayMicroseconds(300);
}

void loop() {
  GetData();
  Serial.print("st");

#if defined (DEBUG)
  for (i1 = 0; i1 < 128; i1++) {
    Adata = AnalogValue1[i1] / 4;
    Serial.write(Adata);
  }
  for (i1 = 0; i1 < 128; i1++) {
    Adata = AnalogValue2[i1] / 4;
    Serial.write(Adata);
  }
    for (i1 = 0; i1 < 128; i1++) {
    Adata = AnalogValue3[i1] / 4;
    Serial.write(Adata);
  }
  for (i1 = 0; i1 < 128; i1++) {
    Adata = AnalogValue4[i1] / 4;
//   Adata=150;
    Serial.write(Adata);
  }
  #else
  for (i1 = 0; i1 < 128; i1++) {
    Adata = AnalogValue1[i1] / 4;
    if(Adata<threshold){Adata=0;}else{Adata=threshold;}
    Serial.write(Adata);
  }
  for (i1 = 0; i1 < 128; i1++) {
    Adata = AnalogValue2[i1] / 4;
        if(Adata<threshold){Adata=0;}else{Adata=threshold;}
    Serial.write(Adata);
  }  
  
#endif


}
