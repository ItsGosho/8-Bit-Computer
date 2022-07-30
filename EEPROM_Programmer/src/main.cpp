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

#define BAUD_RATE 9600

#define MS 1
#define US 2

#define DECIMAL 10
#define HEX 16
#define BINARY 2

void delay(const unsigned int& timeValue, const int& timeUnit);

void clockPin(const uint8_t& pin, const bool& val, const unsigned int& timeValue, const int& timeUnit);

void clockPin(const uint8_t& pin, const bool& val);

template<size_t N>
void pinModes(int (& pins)[N], const bool& mode);

void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("EEPROM Start!");

    digitalWrite(EEPROM_WE_PIN, HIGH);

    int outputPins[4] = {EEPROM_WE_PIN, SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_RCLK_PIN, SHIFT_REGISTER_SR_CLK_PIN};
    pinModes(outputPins, OUTPUT);
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

    clockPin(SHIFT_REGISTER_RCLK_PIN, HIGH);
}

/**
 * Sets the address of the EEPROM and if the EEPROM's output will be enabled.
 */
void setEEPROMPins(const uint16_t& address, const bool& outputEnable) {
    uint16_t shiftOutData = (!outputEnable << 11) | address;

    shiftOutBits(shiftOutData);
}

uint8_t readEEPROMAddress(const uint16_t address) {

    setEEPROMPins(address, true);

    uint8_t byte = 0b00000000;

    for (uint8_t eepromPin = EEPROM_IO_START_PIN; eepromPin <= EEPROM_IO_END_PIN; ++eepromPin) {
        pinMode(eepromPin, INPUT);

        bool readBit = digitalRead(eepromPin);

        if (readBit)
            byte = (byte << 1) | 0b1;
        else
            byte = byte << 1;
    }

    return byte;
}

void printEEPROMAddressDecimal(const uint16_t address) {
    uint8_t addressData = readEEPROMAddress(address);

    char printBuffer[20];
    sprintf(printBuffer, "Address: %d Value: %d", address, addressData);
    Serial.println(printBuffer);
}

void printEEPROMAddressBinary(const uint16_t address) {

    uint8_t addressData = readEEPROMAddress(address);

    bool dataBits[8] = {};

    for (int i = 0; i < 8; ++i)
        dataBits[i] = (0b1 << (7 - i)) & addressData;

    char printBuffer[34];
    sprintf(printBuffer, "Address: %d Value: %d %d %d %d %d %d %d %d", address, dataBits[0], dataBits[1], dataBits[2], dataBits[3], dataBits[4], dataBits[5], dataBits[6], dataBits[7]);
    Serial.println(printBuffer);
}

void printEEPROMAddress(const uint16_t address, const uint8_t format) {

    switch (format) {

        case BINARY:
            return printEEPROMAddressBinary(address);

        case DECIMAL:
            return printEEPROMAddressDecimal(address);

        case HEX:

            break;
    }

}

void setEEPROMAddressData(const uint16_t& address, const uint8_t& data) {


    setEEPROMPins(address, false);

    uint8_t bitIndex = 0;
    for (uint8_t eepromPin = EEPROM_IO_END_PIN; eepromPin >= EEPROM_IO_START_PIN; --eepromPin) {
        pinMode(eepromPin, OUTPUT);
        digitalWrite(eepromPin, ((0b1 << bitIndex) & data) > 0);
        bitIndex++;
    }

    clockPin(EEPROM_WE_PIN, LOW, 1, US);
    delay(10);
}

void loop() {
    setEEPROMAddressData(1, 0b00000001);
    setEEPROMAddressData(2, 0b00000010);
    setEEPROMAddressData(3, 0b11001010);

    printEEPROMAddress(1, BINARY); //1
    printEEPROMAddress(2, BINARY); //2
    printEEPROMAddress(3, BINARY); //202

    delay(1000000);
}

/*---------------- Utils ----------------*/

/**
 * Will set all of the provided @param pins to the given @param mode
 * @param mode Can be 0 (INPUT) or 1 (OUTPUT)
 */
template<size_t N>
void pinModes(int (& pins)[N], const bool& mode) {

    for (size_t i = 0; i < N; ++i)
        pinMode(pins[i], mode);
}

/**
 * Will block the execution of the program for the given time.
 * Time unit can be 1 (MS) or 2 (US). There are macros for easier use.
 */
void delay(const unsigned int& timeValue, const int& timeUnit) {
    switch (timeUnit) {
        case MS:
            return delay(timeValue);
        case US:
            return delayMicroseconds(timeValue);
    }
}

/**
 * Will put the pin LOW or HIGH depending on the @param val for the given time @param timeValue then return it to the opposite @param val
 * Use case for example is when you need to hold a given pin LOW for given time so that the chip can complete given action.
 * @param timeUnit Can be 1 (MS) or 2(US)
 */
void clockPin(const uint8_t& pin, const bool& val, const unsigned int& timeValue, const int& timeUnit) {

    digitalWrite(pin, val);
    delay(timeValue, timeUnit);
    digitalWrite(pin, !val);
}

/**
 * Will put the pin LOW or HIGH depending on the @param val and then return it to the opposite @param val
 * Use case for example is when some action in a chip triggers on the rising edge of a signal and thus you need to bring it LOW then HIGH again
 */
void clockPin(const uint8_t& pin, const bool& val) {
    digitalWrite(pin, val);
    digitalWrite(pin, !val);
}
