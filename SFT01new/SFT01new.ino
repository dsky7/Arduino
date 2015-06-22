#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
//HBee Variables
byte RxBuffer[1024] = {0};
byte IncomingByte = 0;
bool IsEscaped = 0;
int RxN = 0;
byte  RFID1[8] = {0};
int i1, i2, i3;
byte ReceiveData[10];
byte flgStart = 0;
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  //Serial.begin(9600);
  mySerial.begin(9600);
  for (i1 = 0; i1 < 8; i1++) {
    RFID1[i1] = 2 ;
  }
  HBeeSendRFIDtoServer(RFID1);
  i1=0;
}

void loop() {
  // put your main code here, to run repeatedly:
   //   HBeeSendRFIDtoServer(RFID1);

  if (mySerial.available() > 0) {
    //HBeeSendRFIDtoServer(RFID1);
    ReceiveData[i1] = mySerial.read();
    if (flgStart == 0) {
      if (ReceiveData[i1] == 's') flgStart = 1;
    }
    else{i1++;}
  }
  if (i1>7) {
    for (int j = 0; j < 8; j++) {
      RFID1[j] = ReceiveData[i1-8+j];
    }
    i1 = 0;
    flgStart = 0;
    HBeeSendRFIDtoServer(RFID1);
  } 
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//////////////// HBEE SIDE FUNCTIONS //////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

void serialEvent1() {
  ReadIncomingByte();
  FindMessage();
}

void ReadIncomingByte()
{
  while (Serial1.available()) {
    if (RxN < 1024) {
      IncomingByte = Serial1.read();
      if (IncomingByte == 0x7D) IsEscaped = 1;
      else if (IsEscaped) {
        switch (IncomingByte) {
          case 0x5D :
            RxBuffer[RxN] = 0x7D;
            break;
          case 0x5E :
            RxBuffer[RxN] = 0x7E;
            break;
          case 0x31 :
            RxBuffer[RxN] = 0x11;
            break;
          case 0x33 :
            RxBuffer[RxN] = 0x13;
            break;
          default :
            RxBuffer[RxN] = IncomingByte;
            break;
        }
        RxN++;
        IsEscaped = 0;
      }
      else {
        RxBuffer[RxN] = IncomingByte;
        RxN++;
      }
    }
  }
}

void CleanBuffer(int Index) {
  if (Index > 0) {
    for (int j = 0; j < (RxN - Index); j++) RxBuffer[j] = RxBuffer[j + Index];
    RxN -= Index;
  }
}

bool VerifyChecksum(int Length, byte Checksum) {
  byte Sum = 0;
  for (int i = 0; i < Length; i++) Sum += RxBuffer[i + 3];
  if ((Sum + Checksum) == 0xFF) return true;
  return false;
}

void ReadHBeeMessage(byte Message[], int Length) {
  switch (Message[0]) {
    case 0x90 :
      if (Length < 13) return;
      int DataLength = Length - 12;
      byte Data[DataLength];
      for (int i = 0; i < DataLength; i++) Data[i] = Message[i + 12];
      switch (Data[0]) {
        case 0x20 :  // Querry
          if (DataLength < 3) return;
          switch (Data[1]) {
            case 0x10:  // LED
              break;
            case 0x11:  // LED Mode Change
              break;
            case 0x20:  // Sensor
              break;
            case 0x30:  // Motor
              break;
            case 0x40:  // SFT
              switch (Data[3]) {
                case 0x10 :  // Switch the SFT On or Off
                  break;
                case 0x20 :  // Change the SFT Speed
                  break;
                case 0x30 :  // Send Command to SFT
                  mySerial.write(Data[4]);
                  break;
                case 0x40 :  // Send Mode to SFT
                  HBeeSendModeToServer(Data[4]);
                  break;
              }
              break;
          }
          break;
      }
      break;
  }
}


void FindMessage() {
  int MsgLength = 0;
  byte Checksum = 0;
  int i, j = 0;
  while (i < RxN - 1) {
    if (RxBuffer[i] == 0x7E) {
      CleanBuffer(i);
      i = 0;
      if (RxN < 2) return;
      MsgLength = 256 * RxBuffer[1] + RxBuffer[2];
      if (RxN < (MsgLength + 4)) return;
      Checksum = RxBuffer[MsgLength + 3];
      if (VerifyChecksum(MsgLength, Checksum)) {
        byte Message[MsgLength];
        for (j = 0; j < MsgLength ; j++) {
          Message[j] = RxBuffer[j + 3];
        }
        ReadHBeeMessage(Message, MsgLength);
        CleanBuffer(MsgLength + 4);
      }
      else i = 1;
    }
    else i++;
  }
}

byte GetChecksum(byte Message[], int Length) {
  byte Checksum = 0x00;
  byte Sum = 0x00;
  for (int i = 0; i < Length; i++) Sum += Message[i];
  Checksum = 0xFF - Sum;
  return Checksum;
}

int GetNbOfEscapeNeeded(byte Message[], int Length) {
  int NbOfEscapedNeeded = 0;
  for (int i = 0; i < Length; i++) {
    if (Message[i] == 0x7E || Message[i] == 0x7C || Message[i] == 0x11 || Message[i] == 0x13 ) {
      NbOfEscapedNeeded++;
    }
  }
  return NbOfEscapedNeeded;
}

bool GetEscapedMessage(byte Message[], int Length) {
  if (Length > 64) return false;
  byte EscapedMessage[256];
  int j = 0;
  for (int i = 0; i < Length; i++) {
    switch (Message[i]) {
      case 0x11 :
        EscapedMessage[i + j] = 0x7D;
        j++;
        EscapedMessage[i + j] = 0x31;
        break;
      case 0x13 :
        EscapedMessage[i + j] = 0x7D;
        j++;
        EscapedMessage[i + j] = 0x33;
        break;
      case 0x7D :
        EscapedMessage[i + j] = 0x7D;
        j++;
        EscapedMessage[i + j] = 0x5D;
        break;
      case 0x7E :
        EscapedMessage[i + j] = 0x7D;
        j++;
        EscapedMessage[i + j] = 0x5E;
        break;
      default:
        EscapedMessage[i + j] = Message[i];
        break;
    }
  }
  for (int i = 0; i < (Length + j); i++) Message[i] = EscapedMessage[i];
  return true;
}

bool HBeeSendMessage(byte Message[], int Length) {
  byte MyMessage[256];
  MyMessage[0] = 0x00;
  MyMessage[1] = 0x00;
  MyMessage[2] = Length;
  for (int i = 0; i < (Length); i++) MyMessage[i + 3] = Message[i];
  MyMessage[Length + 3] = GetChecksum(Message, Length);
  int FinalLength = Length + 4 + GetNbOfEscapeNeeded(MyMessage, Length + 4);
  if (!GetEscapedMessage(MyMessage, Length + 4)) return false;
  MyMessage[0] = 0x7E;
  Serial1.write(MyMessage, FinalLength);
  return true;
}

bool HBeeSendDataToServer(byte Data[], int Length) {
  byte Message[256];
  Message[0] = 0x10;
  Message[1] = 0x01;
  Message[2] = 0x00;
  Message[3] = 0x13;
  Message[4] = 0xA2;
  Message[5] = 0x00;
  Message[6] = 0x40;
  Message[7] = 0x9E;
  Message[8] = 0x09;
  Message[9] = 0xF3;
  Message[10] = 0xFF;
  Message[11] = 0xFE;
  Message[12] = 0x00;
  Message[13] = 0x00;
  for (int i = 0; i < Length; i++) Message[i + 14] = Data[i];
  if (!HBeeSendMessage(Message, Length + 14)) return false;
  return true;
}

void HBeeSendRFIDtoServer(byte RFID[]) {
  byte Data[16];
  Data[0] = 0x10;
  Data[1] = 0x40;
  Data[2] = 0x01;
  Data[3] = 0x30;
  for (int i = 0; i < 8; i++) Data[i + 4] = RFID[i];
  HBeeSendDataToServer(Data, 12);
}

void HBeeSendModeToServer(byte Mode) {
  byte Data[5];
  Data[0] = 0x10;
  Data[1] = 0x40;
  Data[2] = 0x01;
  Data[3] = 0x40;
  Data[4] = Mode;
  HBeeSendDataToServer(Data, 5);
}

void HBeeSendBusyToServer(bool IsBusy) {
  byte Data[5];
  Data[0] = 0x10;
  Data[1] = 0x40;
  Data[2] = 0x01;
  Data[3] = 0x50;
  Data[4] = IsBusy;
  HBeeSendDataToServer(Data, 5);
}

void HBeeSendBatteryToServer(byte Battery) {
  byte Data[5];
  Data[0] = 0x10;
  Data[1] = 0x40;
  Data[2] = 0x01;
  Data[3] = 0x60;
  Data[4] = Battery;
  HBeeSendDataToServer(Data, 5);
}



