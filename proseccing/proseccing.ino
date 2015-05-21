void setup(){
  Serial.begin(9600);
    pinMode(13, OUTPUT);
}

void loop(){
  Serial.write(100);
  delay(1000);
  Serial.write(200);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  
  
}
