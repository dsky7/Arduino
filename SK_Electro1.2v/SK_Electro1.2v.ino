#include <MsTimer2.h>
int button = 3;
volatile int state = LOW;
int Fmode = 1;
int maxFmode = 7;
int FanSpeedA = 200;
int FanSpeed;
unsigned long currentMillis;
int ledState;
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
byte i1, i2;
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int AvoidChattering = 0;
int FanMode = 0;
int incomingByte = 0;   // for incoming serial data
byte receiveData;
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 15000;           // interval at which to blink (milliseconds)
void setup() {
  // pinMode(analogOutPin, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(0, blink, CHANGE);
  //  MsTimer2::set(500, flash);
  //  MsTimer2::start();

  pinMode(13, OUTPUT);
  pinMode(11, INPUT_PULLUP);
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  i1 = 0;
  TCCR1B &= B11111000;
  TCCR1B |= B00000001;
}
void blink() {
  int buf=0;
  int j;
  if(digitalRead(button) == LOW){buf++;}
  for (j = 0; j < 10; j++) {
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
  }
 
  if(digitalRead(button) == LOW){buf++;}
  for (j = 0; j < 10; j++) {
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
  }

  if(digitalRead(button) == LOW){buf++;}
  for (j = 0; j < 10; j++) {
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
  }

  if(digitalRead(button) == LOW){buf++;}
  for (j = 0; j < 10; j++) {
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
  }

  if(digitalRead(button) == LOW){buf++;}
  for (j = 0; j < 10; j++) {
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
    __asm__("nop\n\t");
  }

Serial.print(buf,DEC);if(digitalRead(button)==LOW){Serial.println("push");}else{Serial.println("-");}
buf=0;
  if (digitalRead(button) == LOW) {
    state = HIGH;
    Fmode++;
    if (Fmode == maxFmode) {
      Fmode = 1;
    }
  } else {
    state = LOW;
  }  digitalWrite(13, state);



}
void flash() {
  static boolean output = HIGH;

  digitalWrite(13, output);
  output = !output;
}

void timerFire() {
  //100ms毎にここが呼び出される
  digitalWrite(13, 1);
  //delay(500);
  //digitalWrite(13, 0);
}

void startFan() {
  for (i1 = 0; i1 < 250; i1++) {
    analogWrite(9, i1);
    delay(10);
  }
}
void GradationStartFan(int maxSpeed) {
  if (maxSpeed > FanSpeed) {
    FanSpeed++;
  }
  Serial.print(FanSpeed, DEC);

  analogWrite(9, FanSpeed);
  delay(10);
}
void GradationStopFan() {
  if (FanSpeed >= 1) {
    FanSpeed--;
  }
  Serial.print(FanSpeed, DEC);
  analogWrite(9, FanSpeed);
  delay(10);
}
void stopFan() {
  for (i1 = 250; i1 > 0; i1--) {
    analogWrite(9, i1);
    delay(10);
  }
}



void loop() {
  int k;
  while (1) {
    switch (Fmode) {
      case 0:
        for (k = 0; k < Fmode; k++) {
          analogWrite(9, FanSpeedA);
          delay(100);
          analogWrite(9, 0);
          delay(200);
        }
        delay(1000);
        break;

      case 1:
        for (k = 0; k < Fmode; k++) {
          analogWrite(9, FanSpeedA);
          delay(100);
          analogWrite(9, 0);
          delay(200);
        }
        delay(1000);
        break;

      case 2:
        for (k = 0; k < Fmode; k++) {
          analogWrite(9, FanSpeedA);
          delay(100);
          analogWrite(9, 0);
          delay(200);
        }
        delay(1000);
        break;


      case 3:
        for (k = 0; k < Fmode; k++) {
          analogWrite(9, FanSpeedA);
          delay(100);
          analogWrite(9, 0);
          delay(200);
        }
        delay(1000);
        break;


      case 4:
        for (k = 0; k < Fmode; k++) {
          analogWrite(9, FanSpeedA);
          delay(100);
          analogWrite(9, 0);
          delay(200);
        }
        delay(1000);
        break;



      case 5:
        for (k = 0; k < Fmode; k++) {
          analogWrite(9, FanSpeedA);
          delay(100);
          analogWrite(9, 0);
          delay(200);
        }
        delay(1000);
        break;

      case 6:
        for (k = 0; k < Fmode; k++) {
          analogWrite(9, FanSpeedA);
          delay(100);
          analogWrite(9, 0);
          delay(200);
        }
        delay(1000);
        break;
    }
  }

  while (1) {
    switch (Fmode) {
      case 0:
        delay(100);
        interrupts();
        analogWrite(9, FanSpeedA);
        randomSeed(analogRead(3));
        delay(random(1, 5000));
        analogWrite(9, 0);
        delay(1000);
        break;
      case 1:
        delay(100);
        interrupts();

        analogWrite(9, FanSpeedA);
        delay(10000);
        analogWrite(9, 0);
        delay(5000);
        break;
      case 2:
        delay(100);
        interrupts();

        analogWrite(9, FanSpeedA);
        delay(30000);
        analogWrite(9, 0);
        delay(5000);
        break;
      case 3:
        delay(100);
        interrupts();

        analogWrite(9, FanSpeedA);
        delay(10000);
        analogWrite(9, 0);
        delay(10000);
        break;
      case 4:
        delay(100);
        interrupts();

        analogWrite(9, FanSpeedA);
        delay(30000);
        analogWrite(9, 0);
        delay(10000);
        break;
      case 5:
        delay(100);
        interrupts();

        analogWrite(9, FanSpeedA);
        randomSeed(analogRead(3));
        delay(random(5000, 45000));
        analogWrite(9, 0);
        delay(random(5000, 45000));
        break;
      case 6:
        delay(100);
        interrupts();

        analogWrite(9, FanSpeedA);
        break;

    }
  }


  while (1) {
    if (digitalRead(11) == 0) {
      FanMode++;
      if (FanMode == 5) {
        FanMode = 0;
      }
      switch (FanMode) {
        case 0:
          digitalWrite(13, 0);
          analogWrite(9, 0);
          break;
        case 1:
          digitalWrite(13, 1);
          analogWrite(9, 150);
          break;
        case 2:
          digitalWrite(13, 0);
          analogWrite(9, 200);
          break;
        case 3:
          digitalWrite(13, 1);
          analogWrite(9, 255);
          break;
        case 4:
          break;
        case 5:
          digitalWrite(13, 1);
          break;
      }
      delay(500);
    }
    delay(10);
    if (FanMode == 4) {
      currentMillis = millis();
      digitalWrite(13, 0);
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (ledState == LOW) {
          ledState = HIGH;
          //      GradationStartFan(255);
          //              analogWrite(9, 255);
        }
        else {
          ledState = LOW;
          //        GradationStopFan();
          //              analogWrite(9, 0);
        }
      }
      if (ledState == 0) {
        GradationStartFan(255);
      } else {
        GradationStopFan();
      }
    }
    delay(10);
  }



  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("Speed:  ");
    //                Serial.println(incomingByte, DEC);
    Serial.print(incomingByte - 48, DEC);
    Serial.println("0%");
    receiveData = (incomingByte - 48) * 25;
  }
  outputValue = i1;
  i1 = i1 + 1;
  // change the analog out value:
  analogWrite(9, receiveData);

  //  Serial.print(sensorValue);

  //Serial.println(outputValue);
  delay(10);
}
