#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#define PIN            7
#define NUMPIXELS      2
#define pinHOLD        5
#define pinFAN        13
#define pinIN          10
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int maxB = 255;
int delayval = 500; // delay for half a second

void setup() {
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setPixelColor(0, pixels.Color(200, 0, 0)); // Moderately bright green color.
  pixels.setPixelColor(1, pixels.Color(200, 0, 0)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  pinMode(pinHOLD, OUTPUT);
  digitalWrite(pinHOLD, LOW);
  pinMode(pinFAN, OUTPUT);
  digitalWrite(pinFAN, LOW);
  pinMode(pinIN, INPUT);
  delay(1000);

  pixels.setPixelColor(0, pixels.Color(100, 100, 0)); // Moderately bright green color.
  pixels.setPixelColor(1, pixels.Color(100, 100, 0)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  digitalWrite(pinFAN, HIGH);
  digitalWrite(pinHOLD, HIGH);
}

void loop() {

  while (1) {
    if (digitalRead(pinIN) == LOW) {
  pixels.setPixelColor(0, pixels.Color(0,200, 0)); // Moderately bright green color.
  pixels.show();
  
    }else{
  pixels.setPixelColor(0, pixels.Color(100,0, 0)); // Moderately bright green color.
  pixels.show();      
    }


  }


  pixels.setPixelColor(0, pixels.Color(0, 200, 0)); // Moderately bright green color.
  pixels.setPixelColor(1, pixels.Color(0, 200, 0)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.

  while (1) {
    digitalWrite(pinHOLD, HIGH); digitalWrite(pinFAN, HIGH);
    for (int i = 0; i < maxB; i++) {
      pixels.setPixelColor(0, pixels.Color(i, 0, i)); // Moderately bright green color.
      pixels.setPixelColor(1, pixels.Color(i, 0, i)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(10);
    }  digitalWrite(pinHOLD, LOW);
    digitalWrite(pinFAN, LOW);
    for (int i = 0; i < maxB; i++) {
      pixels.setPixelColor(0, pixels.Color(maxB - i, 0, maxB - i)); // Moderately bright green color.
      pixels.setPixelColor(1, pixels.Color(maxB - i, 0, maxB - i)); // Moderately bright green color.

      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(10);
    }
  }
  // int ledPWM = 0;
  for (int j; j < 5; j++) {
    for (int i = 0; i < maxB; i++) {
      pixels.setPixelColor(0, pixels.Color(i, 0, i)); // Moderately bright green color.
      pixels.setPixelColor(1, pixels.Color(i, 0, i)); // Moderately bright green color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      //     ledPWM = (int)4 * i;
      //      analogWrite(pinFAN, ledPWM);
      delay(10);
    }
    delay(100); // Delay for a period of time (in milliseconds).
    for (int i = 0; i < maxB; i++) {
      pixels.setPixelColor(0, pixels.Color(maxB - i, 0, maxB - i)); // Moderately bright green color.
      pixels.setPixelColor(1, pixels.Color(maxB - i, 0, maxB - i)); // Moderately bright green color.

      pixels.show(); // This sends the updated pixel color to the hardware.
      //   ledPWM = (int)4 * (maxB - i);
      // analogWrite(pinFAN, ledPWM);
      delay(10);
    }
  }
  // digitalWrite(pinHOLD, LOW);
}
