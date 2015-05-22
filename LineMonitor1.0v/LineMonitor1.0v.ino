

#include <Wire.h>

#include <avr/power.h>

#define PIN 4
#define  LED_Number  8
#define  pi  3.14

byte  LineValue;
byte  i1,i2;
byte  AddressI2C;
float  rad;

void setup()
{
  pinMode(0, INPUT_PULLUP);  
  pinMode(1, INPUT_PULLUP);  
  AddressI2C = digitalRead(0) + 2*digitalRead(1);  
  
  Wire.begin(AddressI2C);                // join i2c bus with address #2
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);

  pinMode(4, OUTPUT);

  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);  
  
  pinMode(13, OUTPUT);
  Serial.println(AddressI2C);
}

void LookLine(void){
 
  int RotationNum=1;
  LineValue=0;
  for(i1=0;i1<8;i1++){
    LineValue = LineValue + RotationNum * digitalRead(5+i1);
    RotationNum=RotationNum<<1;
  }
    

}

void loop()
{
//  LookLine();
  delay(800);
  for (rad = 27; rad <= 27 + LED_Number; rad = rad + 1) {
    for (i2 = 0; i2 < LED_Number; i2++) {
      //strip.setPixelColor(i2, 100, 0, 0);
      strip.setPixelColor(i2, 100 * (sin(2 * pi * rad / 36) + 1) , 100 * (sin(2 * pi * rad / 36) + 1), 100 * (sin(2 * pi * rad / 36) + 1));
    }
    strip.show();
  }

  AddressI2C = digitalRead(0) + 2*digitalRead(1);
  Serial.println(AddressI2C);  
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500*AddressI2C);              // wait for a second
    delay(100);
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  delay(100);  
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{ 
//  Wire.write("hello "); // respond with message of 6 bytes
  Wire.write(LineValue);

  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW

}
