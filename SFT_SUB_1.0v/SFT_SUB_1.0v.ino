#include <RCS620S.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <inttypes.h>
#include <string.h>
#define    WithoutZigBee
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
byte  TransmitData[8];

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

  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(200, 50, 50)); // Moderately bright green color.
    delay(10);
  } pixels.show(); // This sends the updated pixel color to the hardware.
}

void loop()
{
  int ret;
  IDaccord = 0;
  digitalWrite(LED_PIN, HIGH);
  // rcs620s.timeout = COMMAND_TIMEOUT;
  ret = rcs620s.polling();
  for (i1 = 0; i1 < 8; i1++) {
    id[0][i1] = rcs620s.idm[i1];
  }
  for (i3 = 1; i3 < 10; i3++) {
    for (i1 = 0; i1 < 8; i1++) {
      if (id[i3][i1] != id[0][i1]) {
        break;
      }
      if (i1 == 7) {
        IDaccord = 1;
        numRFID = i3;
      }
    }
    if (IDaccord == 1) {
      break;
    }
    if (i3 == 9) {
      numRFID = maxnumRFID + 1;
      maxnumRFID++;
    }
  }
  for (i2 = 0; i2 < 8; i2++) {

#if defined (debug)
    Serial.print(rcs620s.idm[i2]);
    Serial.print(" ");
    #endif
  }
  #if defined(debug)
  Serial.println("");
#endif
  TransmitData[0] = numRFID;
  TransmitData[1] = rcs620s.idm[2];
  TransmitData[2] = rcs620s.idm[3];
#if defined(debug)
  Serial.print(numRFID);
#endif
  for (i1 = 0; i1 < 8; i1++) {
    id[numRFID][i1] = id[0][i1];
#if defined(debug)
    Serial.print(id[numRFID][i1]);
    Serial.print(" ");
#endif
  }
#if defined(debug)
  Serial.println("");
#endif
  //  if (!ret) {
  //    if (waitCardReleased) {
  //      waitCardReleased = 0;
  //    }
  //  } else if (!waitCardReleased) {
  //    // Push
  //    uint8_t data[] = {
  //      0x01, 0x02, 0x2a, 0x00, 0x1d, 0x00, 0x68, 0x74,
  //      0x74, 0x70, 0x3a, 0x2f, 0x2f, 0x74, 0x77, 0x69,
  //      0x74, 0x74, 0x65, 0x72, 0x2e, 0x63, 0x6f, 0x6d,
  //      0x2f, 0x53, 0x44, 0x4b, 0x34, 0x46, 0x65, 0x6c,
  //      0x69, 0x43, 0x61, 0x46, 0x6f, 0x6c, 0x6c, 0x6f,
  //      0x77, 0x20, 0x4d, 0x65, 0x21, 0x21, 0xf1, 0xee,
  //    };
  //    rcs620s.timeout = PUSH_TIMEOUT;
  //    ret = rcs620s.push(data, sizeof(data));
  //    if (ret) {
  //      waitCardReleased = 1;
  //    }
  //  }

  // rcs620s.rfOff();
  digitalWrite(LED_PIN, LOW);
  // delay(POLLING_INTERVAL);
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
  Wire.write(TransmitData, 4);
//  Serial.print("TransmitData[0] = ");
//  Serial.print(TransmitData[0]); Serial.print(" ");
  //#if defined (debug)
  //  Serial.print("TransmitData[0] = ");
  //  Serial.print(TransmitData[0]); Serial.print(" "); Serial.print(TransmitData[1]); Serial.print(" "); Serial.println(TransmitData[2]);     // print the integer
  //#endif
}
