int i1;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  for(i1=0;i1<255;i1++){  Serial.print(i1,DEC);Serial.write(',');Serial.println(255-i1,DEC);  delay(100);}



  // put your main code here, to run repeatedly:
while (Serial.available() > 0) {
  //Serial.write( Serial.read()+1);

  Serial.println("");
  delay(100);
}
}
