#include <RCS620S.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <inttypes.h>
#include <string.h>
//#define    WithoutZigBee
#define    debug
#define    AddSub1    11
#define COMMAND_TIMEOUT  400
#define PUSH_TIMEOUT    2100
#define POLLING_INTERVAL 500
#define NUMPIXELS      24
#define LED_PIN 13
#define PIN            10
RCS620S rcs620s;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int waitCardReleased = 0;
int   i1, i2, i3;
int   id[10][8];
int   maxnumRFID, numRFID, IDaccord;
byte  ReceiveData[8];
int ret;
byte  TransmitData[9];

void setup()
{
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  /*Wire*/
  Wire.begin(AddSub1);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  /*other*/
  digitalWrite(LED_PIN, LOW);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  /*Serial*/
  Serial1.begin(115200);
#if defined(debug)
  Serial.begin(9600);
#endif
  ret = rcs620s.initDevice();
  while (!ret) {}
  digitalWrite(LED_PIN, LOW);

  delay(300);
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(200, 50, 50)); // Moderately bright green color.
    delay(10);
  } pixels.show(); // This sends the updated pixel color to the hardware.
}

void loop()
{
  int ret;
  IDaccord = 0;

  // rcs620s.timeout = COMMAND_TIMEOUT;
  ret = rcs620s.polling();
//#if defined (debug)
//  if (1) {
//    for (i1 = 0; i1 < 8; i1++) {
//      Serial.print(rcs620s.idm[i1],HEX);
//            Serial.print(" ");
//    }
//    Serial.println("");
//  }
//#endif

  for (i1 = 0; i1 < 8; i1++) {
    TransmitData[i1] = rcs620s.idm[i1];
  }
  TransmitData[8] = ret;
  
#if defined (debug)  
    for (i1 = 0; i1 < 8; i1++) {
      Serial.print(TransmitData[i1],HEX);
            Serial.print(" ");
    }
    Serial.print(TransmitData[8],HEX);
    Serial.println("");
#endif

}

void receiveEvent(int howMany)
{
  while (1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    //Serial.print(c);         // print the character
  }
  ReceiveData[0] = Wire.read();    // receive byte as an integer
#if defined(debug)
  Serial.print("ReceiveData[0] = ");
  Serial.println(ReceiveData[0]);         // print the integer
#endif
}

void requestEvent()
{
  Wire.write(TransmitData, 9);
}
