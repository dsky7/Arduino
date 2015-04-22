/*
  Analog input, analog output, serial output

 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.

 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground

 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

// These constants won't change.  They're used to give names
// to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
byte i1,i2;
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)


int incomingByte = 0;   // for incoming serial data
byte receiveData;
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  i1 = 0;
  TCCR1B &= B11111000;
TCCR1B |= B00000001;
}
void startFan(){
  for(i1=0;i1<250;i1++){
  analogWrite(9, i1);
  delay(10);
  }
}
void stopFan(){
  for(i1=250;i1>0;i1--){
  analogWrite(9, i1);
  delay(10);
  }
}
void loop() {
  
  while(1){
  startFan();
  delay(15000);
  stopFan();
  delay(15000);
  
  }
  
  
          if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();

                // say what you got:
                Serial.print("Speed:  ");
//                Serial.println(incomingByte, DEC);
                Serial.print(incomingByte-48,DEC);
                                Serial.println("0%");
                receiveData = (incomingByte-48)*25;
              }
  outputValue = i1;
  i1=i1+1;
  // change the analog out value:
  analogWrite(9, receiveData);

//  Serial.print(sensorValue);

  //Serial.println(outputValue);
  delay(10);
}
