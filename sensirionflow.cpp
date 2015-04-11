/*
 *  Copyright (c) 2015, Johannes Winkelmann, jw@smts.ch
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *      * Neither the name of the <organization> nor the
 *        names of its contributors may be used to endorse or promote products
 *        derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <Wire.h>
#include <Arduino.h>

#include "sensirionflow.h"
#include "flowi2chelper.h"


SensirionFlow::SensirionFlow(uint8_t i2cAddress)
  : mI2cAddress(i2cAddress), mScaleFactor(1)
{
}

void SensirionFlow::init()
{
  const uint8_t CMD_LENGTH = 1;
  const uint8_t CMD_READ_USER_REGISTER[CMD_LENGTH] = { 0xE3 };
  const uint8_t DATA_LENGTH = 6; // 2 data, 1 crc
  uint8_t data[DATA_LENGTH];
  
  // - read user register
  if (!I2CHelper::readFromI2C(mI2cAddress, CMD_READ_USER_REGISTER, CMD_LENGTH, data, 3)) {
    Serial.print("Failed to read from I2C 1\n");
    return;
  }
  
  int16_t baseAddress = (data[0] << 8) + data[1];
  baseAddress &= 0x70; // EEPROM base address is bits <6..4>
  baseAddress >>= 4;
  baseAddress *= 0x300;

  // - read scale factor
  int16_t scaleFactorAddress = (baseAddress + 0x02B6);
  scaleFactorAddress <<= 4;  // address is a left aligned 12-bit value

  uint8_t cmdReadRegister[] = { 0xFA, (scaleFactorAddress >> 8), scaleFactorAddress & 0x00FF };
  if (!I2CHelper::readFromI2C(mI2cAddress, cmdReadRegister, 3, data, DATA_LENGTH)) {
    Serial.print("Failed to read from I2C 2\n");
    return;
  }
  mScaleFactor = (data[0] << 8) + data[1]; // data[2] = crc

  uint16_t measurementUnit = (data[3] << 8) + data[4];  // data[2] = crc
  uint16_t prefix = measurementUnit & 0xF;
  uint16_t timeBase = (measurementUnit >> 4) & 0xF;
  uint16_t volume = (measurementUnit >> 8) & 0x1F;

  timeBase &= 0xF;
  volume   &= 0x1F;

  // todo: store!
}

float SensirionFlow::readSample()
{
  const uint8_t cmdLength = 1;
  const uint8_t dataLength = 2;
  uint8_t command[cmdLength];
  uint8_t data[dataLength];
  
  command[0] = 0xF1;
  if (!I2CHelper::readFromI2C(mI2cAddress, command, 1, data, dataLength)) {
    Serial.print("Failed to read from I2C 4\n");
    return 0;
  }
  
  float measurementValue = ((data[0] << 8) + data[1]);
  return (measurementValue / mScaleFactor);
}

