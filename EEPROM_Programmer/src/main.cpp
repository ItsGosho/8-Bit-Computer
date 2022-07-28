#include <Arduino.h>

/**
 * We need to control a EEPROM (AT28C16), which has total of 21 pins:
 * - 11 pins for the address
 * - 8 pins for the data
 * - 2 pins for controlling it
 * We are using two shift registers. Each of them is 8 bit.
 * That gives us total of 16 bits.
 * The idea is to overcome the problem that Arduino Nano has only 12 digital pins.
 * The shift registers are build using JK Flip Flops. You can store multiple bits in them.
 * The ones that we are using (74HC595) need only 3 pins to be controlled.
 * If we control them with the Arduino we lose 3 pins, but gain 5 more.
 * If we chain two shift registers we lose 3 pins, but gain 13 more.
 * - Note that these additional pins that we achieved with the shift register are perfect only for pins
 * that doesn't require timing.
 */

#define EEPROM_IO_START_PIN 5
#define EEPROM_IO_END_PIN 12
#define EEPROM_WE_PIN 13

#define SHIFT_REGISTER_SER_PIN 2
#define SHIFT_REGISTER_RCLK_PIN 3
#define SHIFT_REGISTER_SR_CLK_PIN 4

void setup() {
    Serial.begin(9600);
    Serial.println("EEPROM Start!");

    pinMode(SHIFT_REGISTER_SER_PIN, OUTPUT);
    digitalWrite(EEPROM_WE_PIN, HIGH);
    pinMode(EEPROM_WE_PIN, INPUT);
    pinMode(SHIFT_REGISTER_RCLK_PIN, OUTPUT);
    pinMode(SHIFT_REGISTER_SR_CLK_PIN, OUTPUT);
}

/**
 * Will shift the bits to the 74HC595 shift register.
 * We are using the Arduino's prebuild shift out function, but it is limited to shifting only 8 bits at a time.
 * Thus we use it twice.
 * The prebuild function will place each of the provided bits on the SER_PIN and clock the SR_CLK pin, which will store them.
 * The shift register has two storages. Shift and Storage Register. The outputed bits from it are from the Storage Register.
 * After storing the 16 bits in the shift register we clock the RCLK_PIN, which moves the stored bits in the Shift Register to the Storage Register.
 */
void shiftOutBits(const uint16_t& bits) {
    uint8_t msb = bits >> 8;
    uint8_t lsb = bits & 0b11111111;

    shiftOut(SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_SR_CLK_PIN, MSBFIRST, msb);
    shiftOut(SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_SR_CLK_PIN, MSBFIRST, lsb);

    digitalWrite(SHIFT_REGISTER_RCLK_PIN, HIGH);
    digitalWrite(SHIFT_REGISTER_RCLK_PIN, LOW);
}

/**
 * Sets the address of the EEPROM and if the EEPROM's output will be enabled.
 */
void setEEPROMPins(const uint16_t& address, const bool& outputEnable) {
    uint16_t shiftOutData = (!outputEnable << 11) | address;

    shiftOutBits(shiftOutData);
}

void printEEPROMAddressData(const uint16_t address) {

    setEEPROMPins(address, true);

    char printBuffer[33];
    bool dataBits[8];

    uint8_t dataBitsIndex = 0;
    for (uint8_t eepromPin = EEPROM_IO_START_PIN; eepromPin <= EEPROM_IO_END_PIN; ++eepromPin) {
        pinMode(eepromPin, INPUT);
        dataBits[dataBitsIndex] = digitalRead(eepromPin);
        dataBitsIndex++;
    }

    sprintf(printBuffer, "Address: %d Bits %d %d %d %d %d %d %d %d", address, dataBits[0], dataBits[1], dataBits[2], dataBits[3], dataBits[4], dataBits[5], dataBits[6], dataBits[7]);
    Serial.println(printBuffer);
}

void setEEPROMAddressData(const uint16_t& address, const uint8_t& data) {

    setEEPROMPins(address, false);

    uint8_t bitIndex = 0;
    for (uint8_t eepromPin = EEPROM_IO_END_PIN; eepromPin >= EEPROM_IO_START_PIN; --eepromPin) {
        pinMode(eepromPin, OUTPUT);
        digitalWrite(eepromPin, ((0b1 << bitIndex) & data) > 0);
        bitIndex++;
    }

    digitalWrite(EEPROM_WE_PIN, LOW);
    delayMicroseconds(1);
    digitalWrite(EEPROM_WE_PIN, HIGH);
    delay(10);
}

void loop() {

    //test1();

    //clearShiftRegister();
/*    shiftOut(SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_SR_CLK_PIN, MSBFIRST, 0b111);
    shiftOut(SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_SR_CLK_PIN, MSBFIRST, 0b11111111);
    //shiftOut(SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_SR_CLK_PIN, MSBFIRST, 0b11010110);
    digitalWrite(SHIFT_REGISTER_RCLK_PIN, HIGH);
    digitalWrite(SHIFT_REGISTER_RCLK_PIN, LOW);*/

    /* for (int i = 1; i <= 2047; ++i) {
         setEEPROMPins(i, true);
         delay(200);
     }*/


    setEEPROMAddressData(2, 0b11001101);
    delay(2000);
    printEEPROMAddressData(2);
    printEEPROMAddressData(2);
    printEEPROMAddressData(2);

  /*  setEEPROMAddressData(1, 0b01110000);
    delay(2000);
    printEEPROMAddressData(1);

    delay(2000);

    printEEPROMAddressData(2);

    delay(2000);

    printEEPROMAddressData(1);*/

    //printRegisterData();
    delay(1000000);

    //shiftOut(SHIFT_REGISTER_SER_PIN,)

}