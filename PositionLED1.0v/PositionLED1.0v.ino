#include <string.h>
char Rdata[20];
int i1=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0) {

    Rdata[i1] = Serial.read();
    if (Rdata[i1] == '\n' || (i1>8) ) {
      i1 = 0;
      Serial.println(Rdata);
      memset(Rdata, 0, sizeof(Rdata));
//      strcpy(Rdata, "123456789");
     // Rdata[0] = "          ";
      
    } else {
      i1++;
    }
    

//    Serial.write( Serial.read() + 1);
  //  Serial.println("");
  }
}
