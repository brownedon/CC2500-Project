/*!
 *  \file    CC2500.cpp
 *  \version 1.0
 *  \date    21-09-2012
 *  \author  Yasir Ali, ali.yasir0@gmail.com
 *
 *  Copyright (c) 2012, Yasir Ali
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program;
 *
 *  \internal
 *    Created: 21-09-2012
 */
 
#include "CC2500.h"
#include "Arduino.h"

#include <SPI.h>
int SPI_DELAY=0;
CC2500::CC2500()
{
}

CC2500::~CC2500()
{
}

void CC2500::init()
{
    // setup pin mode
    pinMode(SS, OUTPUT);
    // disable device
    digitalWrite(SS, HIGH);
    SPI.begin();

    reset();
   
   //FSCTRL1 and MDMCFG4 have the biggest impact on sensitivity...
   
   WriteReg(CC2500_REG_PATABLE, 0x00);
   WriteReg(REG_IOCFG0, 0x01);
   WriteReg(REG_PKTLEN, 0xff);
   WriteReg(REG_PKTCTRL1, 0x04);
   WriteReg(REG_PKTCTRL0, 0x05);
   WriteReg(REG_ADDR, 0x00);
   WriteReg(REG_CHANNR, 0x00);
   //0x0f = -54  rfstudio
   //0x0c = -53  rfstudio example value for sensitivity
   //0x0a = -64  rfstudio example value for sensitivity
   WriteReg(REG_FSCTRL1, 0x0c);  //this controls sensitivity or current usage
   WriteReg(REG_FSCTRL0, 0x00);	
	
   WriteReg(REG_FREQ2, 0x5d);
   WriteReg(REG_FREQ1, 0x44);
   WriteReg(REG_FREQ0, 0xeb);
   
   WriteReg(REG_FREND1, 0xb6);  
   WriteReg(REG_FREND0, 0x10);  

   // Bandwidth
   //0x4a = 406 khz
   //0x5a = 325 khz
   WriteReg(REG_MDMCFG4, 0x4a); 
   WriteReg(REG_MDMCFG3, 0xf8);
   WriteReg(REG_MDMCFG2, 0x73);
   WriteReg(REG_MDMCFG1, 0x23);
   WriteReg(REG_MDMCFG0, 0x3b);
   
   WriteReg(REG_DEVIATN, 0x40);

   WriteReg(REG_MCSM2, 0x07);
   WriteReg(REG_MCSM1, 0x30);
   WriteReg(REG_MCSM0, 0x18);  
   WriteReg(REG_FOCCFG, 0x16); //36
   WriteReg(REG_FSCAL3, 0xa9);
   WriteReg(REG_FSCAL2, 0x0a);
   WriteReg(REG_FSCAL1, 0x00);
   WriteReg(REG_FSCAL0, 0x11);
	
   WriteReg(REG_AGCCTRL2, 0x03);  
   WriteReg(REG_AGCCTRL1, 0x00);
   WriteReg(REG_AGCCTRL0, 0x91);
   //
   WriteReg(REG_TEST2, 0x81);
   WriteReg(REG_TEST1, 0x35); 
   WriteReg(REG_TEST0, 0x0b);  
   
   WriteReg(REG_FOCCFG, 0x0A);		// allow range of +/1 FChan/4 = 375000/4 = 93750.  No CS GATE
   WriteReg(REG_BSCFG, 0x6C);
	
}

void CC2500::reset()
{
  SendStrobe(CC2500_CMD_SRES); 
}


int CC2500::version(){
  return 4;
}
void CC2500::WriteReg(char addr, char value)
{
  digitalWrite(SS, LOW);

  while (digitalRead(MISO) == HIGH) {
  };

  SPI.transfer(addr);
  delay(SPI_DELAY);
  SPI.transfer(value);
  digitalWrite(SS, HIGH);
}

char CC2500::ReadReg(char addr)
{
  addr = addr + 0x80;
  digitalWrite(SS, LOW);
  while (digitalRead(MISO) == HIGH) {
  };
  char x = SPI.transfer(addr);
  delay(SPI_DELAY);
  char y = SPI.transfer(0);
  digitalWrite(SS, HIGH);
  return y;
}

void CC2500::ReadBurstReg(unsigned char addr, unsigned char *buffer, int count)
{
  int i;
  addr = addr | 0xC0; //read burst
  digitalWrite(SS, LOW);
  while (digitalRead(MISO) == HIGH) {};

  char x = SPI.transfer(addr);
  delay(SPI_DELAY);
  for (i = 0; i < count; i++)
  {
    buffer[i] = SPI.transfer(0);
  }
  digitalWrite(SS, HIGH);
}// ReadBurstReg


// For status/strobe addresses, the BURST bit selects between status registers
// and command strobes.
char CC2500::ReadStatusReg(char addr)
{
  addr = addr | 0xC0;
  digitalWrite(SS, LOW);
  while (digitalRead(MISO) == HIGH) {
  };
  char x = SPI.transfer(addr);
  delay(SPI_DELAY);
  char y = SPI.transfer(0);
  digitalWrite(SS, HIGH);
  return y;
}




char CC2500::SendStrobe(char strobe)
{
  digitalWrite(SS, LOW);

  while (digitalRead(MISO) == HIGH) {
  };

  char result =  SPI.transfer(strobe);
  digitalWrite(SS, HIGH);
  delay(SPI_DELAY);
  return result;
}

void CC2500::Read_Config_Regs(void)
{
  Serial.println("Register Configuration");
  Serial.printf("REG_IOCFG0:%x\n", ReadReg(REG_IOCFG0));
  Serial.printf("REG_PKTLEN:%x\n", ReadReg(REG_PKTLEN));
  Serial.printf("REG_PKTCTRL1:%x\n", ReadReg(REG_PKTCTRL1));
  Serial.printf("REG_PKTCTRL0:%x\n", ReadReg(REG_PKTCTRL0));
  Serial.printf("REG_ADDR:%x\n", ReadReg(REG_ADDR));
  Serial.printf("REG_CHANNR:%x\n", ReadReg(REG_CHANNR));
  Serial.printf("REG_FSCTRL1:%x\n", ReadReg(REG_FSCTRL1));
  Serial.printf("REG_FREQ2:%x\n", ReadReg(REG_FREQ2));
  Serial.printf("REG_FREQ1:%x\n", ReadReg(REG_FREQ1));
  Serial.printf("REG_FREQ0:%x\n", ReadReg(REG_FREQ0));
  Serial.printf("REG_MDMCFG4:%x\n", ReadReg(REG_MDMCFG4));
  Serial.printf("REG_MDMCFG3:%x\n", ReadReg(REG_MDMCFG3));
  Serial.printf("REG_MDMCFG2:%x\n", ReadReg(REG_MDMCFG2));
  Serial.printf("REG_MDMCFG1:%x\n", ReadReg(REG_MDMCFG1));
  Serial.printf("REG_MDMCFG0:%x\n", ReadReg(REG_MDMCFG0));
  Serial.printf("REG_DEVIATN:%x\n", ReadReg(REG_DEVIATN));
  Serial.printf("REG_MCSM2:%x\n", ReadReg(REG_MCSM2));
  Serial.printf("REG_MCSM1:%x\n", ReadReg(REG_MCSM1));
  Serial.printf("REG_MCSM0:%x\n", ReadReg(REG_MCSM0));
  Serial.printf("REG_FOCCFG:%x\n", ReadReg(REG_FOCCFG));
  Serial.printf("REG_FSCAL3:%x\n", ReadReg(REG_FSCAL3));
  Serial.printf("REG_FSCAL2:%x\n", ReadReg(REG_FSCAL2));
  Serial.printf("REG_FSCAL1:%x\n", ReadReg(REG_FSCAL1));
  Serial.printf("REG_FSCAL0:%x\n", ReadReg(REG_FSCAL0));
  Serial.printf("REG_TEST2:%x\n", ReadReg(REG_TEST2));
  Serial.printf("REG_TEST1:%x\n", ReadReg(REG_TEST1));
}
