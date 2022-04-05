/*
DISCLAIMER:
By running this script you will alter the sensor's interal data registers.
This is potentially dangerous and will void your sensor's warranty.
Author will not take responsibilty for any potential damage that may be done to your sensors.
Do NOT proceed unless you agree with the following quality assurance clause from the manufacturer and fully understand the potential risks.
"
Caution:
For the digital sensor. User can change the address along with the guide as below link.
Please notice, any data changed by users means users give up the quality assurance terms.
TE will not guarantee the sensor performance any more.
"
*/

#include <Wire.h>

#define SERIAL_BAUD_RATE 115200

// Define your old and new I2C addresses
#define OLD_ADDR 0x28
#define NEW_ADDR 0x36

uint8_t i2c_write_bytes(TwoWire *i2cPtr, uint8_t i2cAddr, uint8_t *arr, uint8_t byteCount);
uint8_t i2c_read_bytes(TwoWire *i2cPtr, uint8_t i2cAddr, uint8_t *arr, uint8_t byteCount);

void setup() {
  Wire.begin();

  // Step 1: Put sensor into command mode within 6ms after power up
  uint8_t command1[3] = {0x0a, 0x00, 0x00};
  uint8_t step1ErrorCode = i2c_write_bytes(&Wire, OLD_ADDR, command1, 3);
  Serial.begin(SERIAL_BAUD_RATE);
  if (!step1ErrorCode) {
    Serial.println(F("Step 1 I2C transaction OK"));
    // Step 2: Request word 02 from EEPROM
    uint8_t command2[3] = {0x02, 0x00, 0x00};
    uint8_t step2ErrorCode = i2c_write_bytes(&Wire, OLD_ADDR, command2, 3);
    if (!step2ErrorCode) {
      Serial.println(F("Step 2 I2C transaction OK"));
      // Step 3: Fetch word 02 from EEPROM
      uint8_t command3[3] = {0x00};
      uint8_t step3ErrorCode = i2c_read_bytes(&Wire, OLD_ADDR, command3, 3);
      if (!step3ErrorCode) {
        Serial.println(F("Step 3 I2C transaction OK."));
        Serial.print(F("Response byte: "));
        Serial.println(command3[0], HEX);
        if (command3[0] == 0x5a) {
          // Step 4: Calculate new word 02
          Serial.print(F("Step 4 excepted response byte received."));
          uint16_t word02 = command3[1] << 8 | command3[2];
          Serial.print(F("Step 4 EEPROM word 02: "));
          Serial.println(word02, BIN);
          word02 &= 0xe007;
          word02 |= NEW_ADDR << 3;
          word02 |= 0xc00;
          Serial.print(F("Step 4 New word 02: "));
          Serial.println(word02, BIN);
          // Step 5: Set new word 02
          uint8_t command5[3] = {0x42, ((word02 & 0xff00) >> 8) , word02 & 0xff};
          uint8_t step5ErrorCode = i2c_write_bytes(&Wire, OLD_ADDR, command5, 3);
          if (!step5ErrorCode) {
            // Step 6: Exit command mode
            Serial.println(F("Step 5 I2C transaction OK."));
            uint8_t command6[3] = {0x80, 0x00, 0x00};
            uint8_t step6ErrorCode = i2c_write_bytes(&Wire, OLD_ADDR, command6, 3);
            if (!step6ErrorCode)  {
              Serial.println(F("Step 6 I2C transaction OK."));
              Serial.print(F("New sensor address is "));
              Serial.println(NEW_ADDR, HEX);
            } else {
              Serial.println(F("Step 6 I2C transaction failed, exiting."));
            }
          } else {
            Serial.println(F("Step 5 I2C transaction failed, exiting."));
          }
        } else {
          Serial.println(F("Step 3 response byte mismatch, exiting."));
        }
        Serial.println(F("Step 3 I2C transaction failed, exiting."));
      }
    } else {
      Serial.println(F("Step 2 I2C transaction failed, exiting."));
    }
  } else {
    Serial.println(F("Step 1 I2C transaction failed, exiting."));
  }
}

void loop() {
  Wire.beginTransmission(OLD_ADDR);
  bool sensorFoundAtOldAddr = Wire.endTransmission() == 0;
  Serial.print(F("Sensor found at old address "));
  Serial.print(OLD_ADDR, HEX);
  Serial.println(sensorFoundAtOldAddr ? " Yes" : " No");
  Wire.beginTransmission(NEW_ADDR);
  bool sensorFoundAtNewAddr = Wire.endTransmission() == 0;
  Serial.print(F("Sensor found at new address "));
  Serial.print(NEW_ADDR, HEX);
  Serial.println(sensorFoundAtNewAddr ? " Yes" : " No");
  delay(1000);
}

uint8_t i2c_write_bytes(TwoWire *i2cPtr, uint8_t i2cAddr, uint8_t *arr, uint8_t byteCount)
{
  i2cPtr->beginTransmission(i2cAddr);
  for (uint8_t i = 0; i < byteCount; i++)
  {
    i2cPtr->write(*(arr + i));
  }
  return (uint8_t)i2cPtr->endTransmission();
}

uint8_t i2c_read_bytes(TwoWire *i2cPtr, uint8_t i2cAddr, uint8_t *arr, uint8_t byteCount)
{
  i2cPtr->requestFrom(i2cAddr, byteCount);
  for (uint8_t i = 0; i < byteCount; i++)
  {
    *(arr + i) = i2cPtr->read();
  }
  return (uint8_t)i2cPtr->endTransmission();
}
