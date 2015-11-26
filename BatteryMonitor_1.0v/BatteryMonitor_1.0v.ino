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
  pinMode(13, OUTPUT);
  pinMode(11, INPUT_PULLUP);
  
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  i1 = 0;
  TCCR1B &= B11111000;
  TCCR1B |= B00000001;
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
  
  
int Vbattery;
 while(1){
   Vbattery = analogRead(A0);
   Serial.println(Vbattery);
   delay(1000);
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
      if(ledState==0){
                GradationStartFan(255);}else{
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
