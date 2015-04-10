

#include <cc2500.h>
#include <cc2500_REG.h>
#include <stdio.h>
#include <string.h>
#include <SPI.h>
#include <RFduinoBLE.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

long rawcount1 = 0;
long rawcount2 = 0;
int firstTime = true;
//
//
#define BTLE_BATTERY             0x07 //btle battery
#define TRANSMITTER_FULL_PACKET  0x0F //Transmitter Full Packet

typedef struct _Dexcom_packet
{
  uint32_t dest_addr;
  uint32_t src_addr;
  uint8_t  port;
  uint8_t  device_info;
  uint8_t  txId;
  uint16_t raw;
  uint16_t filtered;
  uint8_t  battery;
  uint8_t  unknown;
  uint8_t  checksum;
  uint8_t  RSSI;
  uint8_t  LQI;
} Dexcom_packet;

uint8_t packet[40];
uint8_t oldPacket[40];

static const int NUM_CHANNELS = 4;
static uint8_t nChannels[NUM_CHANNELS] = { 0, 100, 199, 209 };
int8_t fOffset[NUM_CHANNELS] = {0xe4, 0xe3, 0xe2, 0xe2};
const int GDO0_PIN = 2;     // the number of the GDO0_PIN pin

CC2500 cc2500;

void setup()
{
  Serial.begin(9600);
  Serial.println("*************Restart");
  pinMode(GDO0_PIN, INPUT);
  cc2500.init();
  // this is the data we want to appear in the advertisement
  // (the deviceName length plus the advertisement length must be <= 18 bytes)
  RFduinoBLE.advertisementData = "Glucose";
  RFduinoBLE.deviceName = "CGMS MICRO1";
  RFduinoBLE.advertisementInterval = SECONDS(1);
  RFduinoBLE.txPowerLevel = 4;  // (-20dbM to +4 dBm)

  // start the BLE stack
  RFduinoBLE.begin();

  analogReference(VBG); // Sets the Reference to 1.2V band gap
  analogSelection(VDD_1_3_PS);  //Selects VDD with 1/3 prescaling as the analog source
  memset(&packet, 0, sizeof(packet));
}

void loop()
{
  int waitTime = RxData_RF();

  firstTime = false;
  cc2500.SendStrobe(CC2500_CMD_SPWD);
  //just not sure about the adjustment until further testing
  RFduino_ULPDelay(SECONDS(int(297 - waitTime)));
  delay(50);
  cc2500.SendStrobe(CC2500_CMD_SIDLE);
  while ((cc2500.ReadStatusReg(REG_MARCSTATE) & 0x1F) != 0x01) {};
  delay(50);
  Serial.println("Wake");
}


void swap_channel(uint8_t channel, uint8_t newFSCTRL0)
{
  cc2500.WriteReg(REG_CHANNR, channel);
  cc2500.WriteReg(REG_FSCTRL0, newFSCTRL0);

  cc2500.SendStrobe(CC2500_CMD_SRX);
  while ((cc2500.ReadStatusReg(REG_MARCSTATE) & 0x1F) != 0x0D) {};
}

long RxData_RF(void)
{
  int Delay = 0;
  int packetFound = 0;
  int channel = 0;
  int crc = 0;
  int lqi = 0;
  uint8_t PacketLength;
  uint8_t freqest;
  Dexcom_packet Pkt;
  long timeStart = 0;
  int continueWait = false;
  Serial.print("Start Listening:");
  Serial.println(millis());

  while (!packetFound && channel < 4) {
    //
    continueWait = false;
    swap_channel(nChannels[channel], fOffset[channel]);
    timeStart = millis();
    //
    Serial.print("Channel:");
    Serial.println(channel);

    //wait on this channel until we receive something
    //if delay is set, wait for a short time on each channel
    Serial.print("Delay:");
    Serial.println(Delay);
    while (!digitalRead(GDO0_PIN) && (millis() - timeStart < Delay) || (!digitalRead(GDO0_PIN) && Delay == 0))
    {
      delay(1);
    }
    if (digitalRead(GDO0_PIN)) {
      Serial.println("Got packet");
      PacketLength = cc2500.ReadReg(CC2500_REG_RXFIFO);
      //
      Serial.print("Wait:");
      Serial.println(millis() - timeStart);
      Serial.println(PacketLength);
      //if packet length isn't 18 skip it
      if (PacketLength == 18) {
        //keep the values around in oldPacket so there's something to return over BLE
        //if packet capture fails crc check
        memcpy(oldPacket, packet, 18);
        //
        cc2500.ReadBurstReg(CC2500_REG_RXFIFO, packet, PacketLength);
        memcpy(&Pkt, packet, 18);
        //you should have
        //first byte, not in this array, packet length (18)
        //packet#  sample  comment
        //0        FF      dest addr
        //1        FF      dest addr
        //2        FF      dest addr
        //3        FF      dest addr
        //4        CA      xmtr id
        //5        4C      xmtr id
        //6        62      xmtr id
        //7        0       xmtr id
        //8        3F      port
        //9        3       hcount
        //10       93      transaction id
        //11       93      raw isig data
        //12       CD      raw isig data
        //13       1D      filtered isig data
        //14       C9      filtered isig data
        //15       D2      battery
        //16       0       fcs(crc)
        //17       AD      fcs(crc)
        //
        //
        Pkt.LQI = cc2500.ReadStatusReg(REG_LQI);
        crc = Pkt.LQI & 0x80;

        //packet is good
        if (crc == 128) {
          //packet has the correct transmitter id
          if (packet[4] == 0xca && packet[5] == 0x4c && packet[6] == 0x62 && packet[7] == 0x00) {
            Pkt.RSSI = cc2500.ReadStatusReg(REG_RSSI);
            lqi = (int)(Pkt.LQI & 0x7F);
            Serial.print("RSSI:");

            if ((int)Pkt.RSSI >= 128)
              Serial.println((((int)Pkt.RSSI - 256) / 2 - 73), DEC);
            else
              Serial.println(((int)Pkt.RSSI / 2 - 73), DEC);
            
            //
            convertFloat();
            freqest = cc2500.ReadStatusReg(REG_FREQEST);            
            fOffset[channel] += freqest;            
            Serial.print("Offset:");
            Serial.println(fOffset[channel], DEC);
            
            packetFound = 1;
          } else {
            continueWait = false;
            Serial.println("Another dexcom found");
          }
        } else {
          memcpy(packet, oldPacket, 18);
          continueWait = false;
          Serial.println("CRC Failed");
        }
      } else {
        memcpy(packet, oldPacket, 18);
        continueWait = true;
        Serial.println("Packet length issue");
        //go to next channel and camp out there
        if (channel < 3) {
          channel++;
        } else {
          channel = 0;
        }
        Delay = 0;
      }  //packet length=18
    }

    if (!continueWait) {
      channel++;
      Delay = 600;
    }

    // Make sure that the radio is in IDLE state before flushing the FIFO
    // (Unless RXOFF_MODE has been changed, the radio should be in IDLE state at this point)
    cc2500.SendStrobe(CC2500_CMD_SIDLE);
    while ((cc2500.ReadStatusReg(REG_MARCSTATE) & 0x1F) != 0x01) {};

    // Flush RX FIFO
    cc2500.SendStrobe(CC2500_CMD_SFRX);
  }
  Serial.print("End Listening:");
  Serial.println(millis());

  //add one additional second(per channel) to the delay 
  return channel - 1;
}// Rf RxPacket


//String based method to turn the ISIG packets into a binary string
//also need to add back the left hand zeros if any
//much more elegant method in dexterity, but on arduino I get a bad value every 24 hours or so
//due to (assumed)loss of leading zeros
void convertFloat() {
  String result;
  result = lpad(String(packet[11], BIN), 8);
  result += lpad(String(packet[12], BIN), 8);
  result += lpad(String(packet[13], BIN), 8);
  result += lpad(String(packet[14], BIN), 8);

  char ch[33];
  char reversed[31];
  result.toCharArray(ch, 33);

  int j = 0;
  for (int i = 31; i >= 0; i--) {
    reversed[j] = ch[i];
    j++;
  }

  String reversed_str = String(reversed);
  String exp1 = reversed_str.substring(0, 3);
  int exp1_int = (int)binStringToLong(exp1);

  String mantissa1 = reversed_str.substring(3, 16);
  long mantissa1_flt = binStringToLong(mantissa1);

  String exp2 = reversed_str.substring(16, 19);
  int exp2_int = (int)binStringToLong(exp2);

  String mantissa2 = reversed_str.substring(19, 32);
  long mantissa2_flt = binStringToLong(mantissa2);

  rawcount1 = (mantissa1_flt * pow(2, exp1_int) * 2);
  rawcount2 = mantissa2_flt * pow(2, exp2_int);
  
  Serial.print("Raw ISIG:");
  Serial.println(rawcount2);
}

//add zeros back to a string representation of a binary number
//ex.  111 should be 00000111 or the calculations in convertFloat
//would go haywire
String lpad(String str, int length) {
  String zeroes;
  int len = length - str.length();
  while (len > 0) {
    zeroes = zeroes + "0";
    len--;
  }
  return zeroes + str;
}

long binStringToLong(String binary) {
  long result = 0;
  long power = 1;
  char ch[14];

  binary.toCharArray(ch, binary.length() + 1);
  for (int j = binary.length() - 1; j >= 0; j--) {
    if (ch[j] == '1') {
      result = result + power;
    }
    power = power * 2;
  }
  return result;
}

void RFduinoBLE_onReceive(char * data, int len)
{
  Serial.println("RFduinoBLE_onReceive");
  Serial.println(data[0], HEX);
  switch (data[0]) {
    case BTLE_BATTERY: {
        int sensorValue = analogRead(1); // the pin has no meaning, it uses VDD pin
        float batteryVoltage = sensorValue * (3.6 / 1023.0); // convert value to voltage
        char ch[3];
        ch[0] = BTLE_BATTERY;
        ch[1] = (byte) ((int)(batteryVoltage * 100) & 0xFF);
        ch[2] = (byte) (((int)(batteryVoltage * 100) >> 8) & 0xFF);
        RFduinoBLE.send(ch, 3);

        break;
      } 

    case TRANSMITTER_FULL_PACKET: {
        char ch[6];
        ch[0] = TRANSMITTER_FULL_PACKET;
        
        ch[1] = packet[11];
        ch[2] = (int)((rawcount2 >> 24) & 0xFF) ;
        ch[3] = (int)((rawcount2 >> 16) & 0xFF) ;
        ch[4] = (int)((rawcount2 >> 8) & 0XFF);
        ch[5] = (int)((rawcount2 & 0XFF));
        RFduinoBLE.send(ch, 6);
        break;
      }
  }
}

