#include <EEPROM.h>
#include <Wire.h>

#define DEBUG
#define DelayClock  1
#define  AddWhite  128
#define  AddBlack  512
int        maskFR = 40, maskRL = 50;
const int analogInPin1 = A0, analogInPin2 = A1, analogInPin3 = A8, analogInPin4 = A10;
const int CLK = 4;
const int SI = 11;
const int CaliLED = 0;
const int CaliButton = 1;
int    time1, time2;
long    LoopTime, oldLoopTime;
int state = 0;
int  value[128];
int    adata;
int  threshold = 100, thresholdLOW, thresholdHIGH;
byte Adata;
byte sendData[8], TransmitData[4];
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int AnalogValue1[128], AnalogValue2[128], AnalogValue3[128], AnalogValue4[128];
int Data[32];
int i1, i2, i3;
int LineQuantity = 0;
long aveLinePos;
void sendPC(int *temp);
void maskData(int *temp, int maskTH);
void sendPC_Debug(int *temp);
void sendPC_Byte(int *temp);

void setup() {
  Serial.begin(57600);
  pinMode(CLK, OUTPUT);
  pinMode(SI, OUTPUT);
  pinMode(CaliLED, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(CaliButton, INPUT_PULLUP);
  attachInterrupt(3, interruptsw, LOW ); // (1)
  digitalWrite(CLK, LOW);
  Wire.begin(4);
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  ADCSRA = ADCSRA & 0xf8;
  ADCSRA = ADCSRA | 0x04;
}

void GetDataOne(int *temp , int pinNum) {
  digitalWrite(SI, HIGH);
  delayMicroseconds(DelayClock);

  for (i1 = 0; i1 < 128; i1++) {
    digitalWrite(CLK, HIGH);
    delayMicroseconds(DelayClock);

    digitalWrite(SI, LOW);

    temp[i1] = analogRead(pinNum);
    digitalWrite(CLK, LOW);
    delayMicroseconds(DelayClock);

  }
  digitalWrite(CLK, HIGH);
  delayMicroseconds(DelayClock);
  digitalWrite(CLK, LOW);
  delayMicroseconds(20);
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
  //  delayMicroseconds(300);
}

void CalibrationBlack(void) {
  for (i1 = 0; i1 < 128; i1++) {
    EEPROM.write(AddBlack + i1,  AnalogValue1[i1] / 4); //AnalogValue1[i1]
  }
  for (i1 = 0; i1 < 128; i1++) {
    value[i1] = EEPROM.read(AddBlack + i1) * 4;
  }

  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
}

int CalculateLine(int *AValue, int mask) {
  LineQuantity = 0; aveLinePos = 0;
  for (i1 = mask; i1 < 128 - mask; i1++) {
    if (AValue[i1] / 4 < threshold) {
      LineQuantity ++;
      aveLinePos = aveLinePos + i1;
    }
  }
  if (LineQuantity == 0) {
    return 21;
  }
  return (int)aveLinePos / LineQuantity;
}
void CalibrationWhite(void) {
  //GetData();

  for (i1 = 0; i1 < 128; i1++) {
    EEPROM.write(AddWhite + i1, AnalogValue1[i1] / 4) ;
  }

  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
}


void loop() {
  LoopTime = millis() - oldLoopTime;
  oldLoopTime = millis();
  GetDataOne(AnalogValue1, analogInPin1);
  time1 = millis();

#if defined (DEBUG)
  Serial.print("st");
  sendPC_Debug(AnalogValue1);
#endif
  GetDataOne(AnalogValue2, analogInPin2);
#if defined (DEBUG)
  sendPC_Debug(AnalogValue2);
#endif
  GetDataOne(AnalogValue3, analogInPin3);
#if defined (DEBUG)
  sendPC_Debug(AnalogValue3);
#endif
  GetDataOne(AnalogValue4, analogInPin4);
#if defined (DEBUG)
  sendPC_Debug(AnalogValue4);
#endif
  time2 = millis();
  Data[0] = LoopTime;
  Data[1] = threshold;
  Data[2] = maskFR;
  Data[3] = maskRL;

  Data[4] = CalculateLine(AnalogValue1, maskFR);
  Data[5] = CalculateLine(AnalogValue2, maskFR);
  Data[6] = CalculateLine(AnalogValue3, maskRL);
  Data[7] = CalculateLine(AnalogValue4, maskRL);
  TransmitData[0] = Data[4];
  TransmitData[1] = Data[5];
  TransmitData[2] = Data[6];
  TransmitData[3] = Data[7];
#if defined DEBUG
  for (i1 = 0; i1 < 4; i1++) {
    sendData[i1] = Data[i1 + 4];
  }
    sendPC_Byte(Data);
#endif

  if (state == 1) {
    delay(100);

    CalibrationBlack();

    digitalWrite(CaliLED, 1);
    delay(1000);
    digitalWrite(CaliLED, 0);
    state = 2;
    attachInterrupt(3, interruptsw, LOW );
  } else if (state == 3) {

    delay(100);
    CalibrationWhite();

    digitalWrite(CaliLED, 1);
    delay(100);
    digitalWrite(CaliLED, 0);
    delay(100);
    digitalWrite(CaliLED, 1);
    delay(100);
    digitalWrite(CaliLED, 0);
    delay(100);

    state = 0;
    attachInterrupt(3, interruptsw, LOW );
  }

}
void maskData(int *temp, int maskTH) {
  for (i1 = 0; i1 < maskTH; i1++) {
    temp[i1] = 1023;
  }
  for (i1 = 128 - maskTH; i1 < 128; i1++) {
    temp[i1] = 1023;
  }
}
void sendPC_Debug(int *temp) {

  for (i1 = 0; i1 < 128; i1++) {
    Adata = temp[i1] / 4;
    Serial.write(Adata);
  }
}
void sendPC_Byte(int *temp) {
  for (i1 = 0; i1 < 32; i1++) {
    Adata = temp[i1];
    Serial.write(Adata);
  }
}
void sendPC(int *temp) {

  for (i1 = 0; i1 < 128; i1++) {
    Adata = temp[i1] / 4;
    if (Adata < threshold) {
      Adata = 0;
    } else {
      Adata = threshold;
    }
    Serial.write(Adata);
  }
}
void interruptsw()
{
  detachInterrupt(3);
  state  ++;
}
void requestEvent()
{
  Wire.write(TransmitData, 4);
}
void receiveEvent(int howMany)
{
  i3 = 0;

  while (0 < Wire.available()) // loop through all but the last
  {
    Data[8 + i3] = Wire.read(); // receive byte as a character
    i3++;
  }
}
