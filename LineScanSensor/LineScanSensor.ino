#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define DelayClock  20
#define PIN            6
#define NUMPIXELS      8
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
const int CLK = 0;
const int SI = 1;
byte Adata;
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int AnalogValue[128];
int i1;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(CLK, OUTPUT);
  pinMode(SI, OUTPUT);
  digitalWrite(CLK, LOW);
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
    for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    if(i<(NUMPIXELS/2)){    pixels.setPixelColor(i, pixels.Color(200*(4-i)/4,200,200));}else{pixels.setPixelColor(i, pixels.Color(200-(NUMPIXELS-1-i)*25,0,200));}



  }
      pixels.show(); // This sends the updated pixel color to the hardware.

}



void GetData(void){
  digitalWrite(SI, HIGH);
  delayMicroseconds(DelayClock);

  for (i1 = 0; i1 < 128; i1++) {
    // read the analog in value:
    digitalWrite(CLK, HIGH);
    delayMicroseconds(DelayClock);
    
  digitalWrite(SI, LOW);

    AnalogValue[i1] = analogRead(analogInPin);
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
  //delay(1);
//  Adata = (byte)AnalogValue[0]/4;
for(i1 = 0;i1<128;i1++){
  Adata = AnalogValue[i1]/4;
  Serial.write(Adata);
//    Serial.print(Adata,DEC);
}
Serial.println("");
//delay(100);
  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
 // delay(2);
}
