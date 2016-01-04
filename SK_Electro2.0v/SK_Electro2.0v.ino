#include <TimerOne.h>
int switch1 = 3;
int LED = 13;
int Gate = 6;
int OnTime = 10, OffTime = 5;
int ONOFF = 1;
int Fmode = 1;
int cntOn, cntOff;
int frandom = 0;
void setup()
{
  pinMode(switch1, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(Gate, OUTPUT);
  Timer1.initialize(1000000); // 8s ga max kana  ;set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
  analogWrite(Gate, 200);
}

void loop()
{

  /* analogWrite(6,200);analogWrite(3,200);analogWrite(11,200);delay(1000);
   analogWrite(6,0);analogWrite(3,0);analogWrite(11,0);delay(1000);*/
  if (digitalRead(switch1) == LOW) {
    digitalWrite(LED, HIGH);
    delay(300);
    digitalWrite(LED, LOW);
    delay(300);    
    Fmode++;
    if (Fmode == 7) {
      Fmode = 1;
    }

    switch (Fmode) {
      case 1:
        OnTime = 10;
        OffTime = 5;
        break;

      case 2:
        OnTime = 30;
        OffTime = 5;
        break;

      case 3:
        OnTime = 10;
        OffTime = 10;
        break;


      case 4:
        OnTime = 30;
        OffTime = 10;
        frandom = 0;
        break;

      case 5:
        OnTime = 1;
        OffTime = 1;
        break;

      case 6:
        OnTime = 10;
        OffTime = 0;
        break;
    }

  } else {
    digitalWrite(LED, LOW);
  }

}

/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
  if (Fmode==6) {analogWrite(Gate, 200);} else {
    if (Fmode == 5) {
      if (frandom == 0) {
        randomSeed(analogRead(0));
        OffTime = random(5, 45);
        OnTime = random(5, 45);
        frandom = 1;
      }
    }
    if (ONOFF == 0) {
      if (cntOff < OffTime) {
        cntOff++;
      } else {
        cntOff = 0;
        ONOFF = 1;
        analogWrite(Gate, 200);
      }

    } else {
      if (cntOn < OnTime) {
        cntOn++;
      } else {
        cntOn = 0;
        ONOFF = 0;
        analogWrite(Gate, 0);
        frandom = 0;
      }
    }
  }
}
