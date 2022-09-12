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

/**
 * Each index corresponds to the digit.
 * Index 0 = Digit 0
 * Index 1 = Digit 9
 * Up to Index 9.
 *
 * (7) HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW = 0b00110010
 * (8) HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH = 0b11111110
 * (9) HIGH, LOW, LOW, HIGH, HIGH, HIGH, HIGH = 0b11110010
 */
byte segmentDisplayMappingDigits[10] = {0b01111110, 0b00010010, 0b10111100, 0b10110110, 0b11010010, 0b11100110, 0b11101110, 0b00110010, 0b11111110, 0b11110010};

/**
 * Index 0 = NOTHING
 * Index 1 = -
 */
byte segmentDisplayCharacterMapping[2] = {0b00000000, 0b10000000};

void digitalWriteBetween(const unsigned int& startPin, const unsigned int& endPin, const unsigned int& value);

unsigned int digitalReadBetween(const unsigned int& startPin, const unsigned int& endPin);

void delay(const unsigned int& timeValue, const int& timeUnit);

void clockPin(const uint8_t& pin, const bool& val, const unsigned int& timeValue, const int& timeUnit);

void clockPin(const uint8_t& pin, const bool& val);

template<size_t N>
void pinModes(int (& pins)[N], const bool& mode);

void setShiftRegisterBits(const uint16_t& bits);

void setEEPROMPins(const uint16_t& address, const bool& outputEnable);

uint8_t readEEPROMAddress(const uint16_t& address);

void printEEPROMAddressBinary(const uint16_t& address);

void printEEPROMAddressDecimal(const uint16_t& address);

void printEEPROMAddressHex(const uint16_t& address);

void printEEPROMAddress(const uint16_t& address, const uint8_t& format);

void printEEPROMAddress(const uint16_t& from, const uint16_t& to, const uint8_t& format);

void setEEPROMAddressData(const uint16_t& address, const uint8_t& data);

void programEEPROM3BitsSegmentDecoder();

void programEEPROM8BitsSegmentDecoder();

void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("EEPROM Start!");

    digitalWrite(EEPROM_WE_PIN, HIGH);

    int outputPins[4] = {EEPROM_WE_PIN, SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_RCLK_PIN, SHIFT_REGISTER_SR_CLK_PIN};
    pinModes(outputPins, OUTPUT);

    Serial.println("Started programming!");
    programEEPROM8BitsSegmentDecoder();
    Serial.println("Finished programming!");


}

/*TODO: CPU EEPROM Programming Logic
 * We use two EEPROMs, because the control signals are too much.
*
* Instruction Unique Code:
 * LDA : 0001
 * ADD : 0010
 * OUT : 0011
 * HLT : 0100
*
* Address:
* A0/3 Instruction (4 bits)
* A4/6: Microinstruction (3 bits)
* A7/11: Nothing Yet (5 bits all 0)
*
* Data:
* First EEPROM: HLT, MI, RI, RO, IO, II, AI, AO
* Second EEPROM: EO, SU, BI, OI, CE, CO, J, NOTHING (0)
 *
 * Example:
 *
 * Instruction | N Microinstruction | Activate Control Signals (HLT, MI, RI, RO, IO ...)
 *
 * 0001             000                 0100000000000010
 * 0001             001                 0001010000000100
 * 0001             010                 0100100000000000
 * 0001             011                 0001001000000000
 * */

void programFirstEEPROM() {
    //First EEPROM LDA INSTRUCTION
    //Address 0bA11->A0
    setEEPROMAddressData(0b000000010001, 0b00000010); //Instruction LDA; Microinstruction: 1;
    setEEPROMAddressData(0b000000100001, 0b00101000); //Instruction LDA; Microinstruction: 2;
    setEEPROMAddressData(0b000000110001, 0b00010010); //Instruction LDA; Microinstruction: 3;
    setEEPROMAddressData(0b000001000001, 0b01001000); //Instruction LDA; Microinstruction: 4;

    //First EEPROM ADD INSTRUCTION
    setEEPROMAddressData(0b000000010010, 0b00000010); //Instruction LDA; Microinstruction: 1;
    setEEPROMAddressData(0b000000100010, 0b00101000); //Instruction LDA; Microinstruction: 2;
    setEEPROMAddressData(0b000000110010, 0b00010010); //Instruction LDA; Microinstruction: 3;
    setEEPROMAddressData(0b000001000010, 0b00001000); //Instruction LDA; Microinstruction: 4;
    setEEPROMAddressData(0b000001010010, 0b01000000); //Instruction LDA; Microinstruction: 5;

    //First EEPROM OUT INSTRUCTION
    setEEPROMAddressData(0b000000010011, 0b01000000); //Instruction LDA; Microinstruction: 1;
    setEEPROMAddressData(0b000000100011, 0b00010100); //Instruction LDA; Microinstruction: 2;
    setEEPROMAddressData(0b000000110011, 0b00000001); //Instruction LDA; Microinstruction: 3;

    //First EEPROM HLT INSTRUCTION
    setEEPROMAddressData(0b000000010100, 0b00000010); //Instruction LDA; Microinstruction: 1;
    setEEPROMAddressData(0b000000100100, 0b00101000); //Instruction LDA; Microinstruction: 2;
    setEEPROMAddressData(0b000000110100, 0b00000001); //Instruction LDA; Microinstruction: 3;
}

void programSecondEEPROM() {
    //First EEPROM LDA INSTRUCTION
    setEEPROMAddressData(0b000000010001, 0b01000000); //Instruction LDA; Microinstruction: 1;
    setEEPROMAddressData(0b000000100001, 0b00100000); //Instruction LDA; Microinstruction: 2;
    setEEPROMAddressData(0b000000110001, 0b00000000); //Instruction LDA; Microinstruction: 3;
    setEEPROMAddressData(0b000001000001, 0b00000000); //Instruction LDA; Microinstruction: 4;

    //First EEPROM ADD INSTRUCTION
    setEEPROMAddressData(0b000000010010, 0b01000000); //Instruction LDA; Microinstruction: 1;
    setEEPROMAddressData(0b000000100010, 0b00100000); //Instruction LDA; Microinstruction: 2;
    setEEPROMAddressData(0b000000110010, 0b00000000); //Instruction LDA; Microinstruction: 3;
    setEEPROMAddressData(0b000001000010, 0b00000100); //Instruction LDA; Microinstruction: 4;
    setEEPROMAddressData(0b000001010010, 0b00000001); //Instruction LDA; Microinstruction: 5;

    //First EEPROM OUT INSTRUCTION
    setEEPROMAddressData(0b000000010011, 0b01000000); //Instruction LDA; Microinstruction: 1;
    setEEPROMAddressData(0b000000100011, 0b00100000); //Instruction LDA; Microinstruction: 2;
    setEEPROMAddressData(0b000000110011, 0b00001000); //Instruction LDA; Microinstruction: 3;

    //First EEPROM HLT INSTRUCTION
    setEEPROMAddressData(0b000000010100, 0b01000000); //Instruction LDA; Microinstruction: 1;
    setEEPROMAddressData(0b000000100100, 0b00100000); //Instruction LDA; Microinstruction: 2;
    setEEPROMAddressData(0b000000110100, 0b00000000); //Instruction LDA; Microinstruction: 3;
}

void loop() {
    /*   setEEPROMAddressData(1, 0b00000001);
       setEEPROMAddressData(2, 0b00000010);
       setEEPROMAddressData(3, 0b11001010);

       printEEPROMAddress(1, BINARY); //1
       printEEPROMAddress(2, BINARY); //2
       printEEPROMAddress(3, BINARY); //202

       delay(1000000);*/
}

/*    int number = 105;

bool isPositive = number >= 0;

number = abs(number);
int combination0Address = (0b00 << 8) | number; //correct
int combination1Address = (0b10 << 8) | number; // correct
int combination2Address = (0b01 << 8) | number; // correct
int combination3Address = (0b11 << 8) | number; // correct

int onesDigit = number % 10;
int secondDigit = (number / 10) % 10;
int thirdDigit = (number / 100) % 10;*/

/*
 * We have 8 bit number. It implements two's complement.
 * That means 1 bit for sign and 7 bit for the number.
 * 1000 0000 = -128
 * 1111 1111 = -1
 * 0000 0000 = 0
 * 0111 1111 = 127
 * */

void programEEPROM8BitsSegmentDecoder() {

    for (int programNumber = -128; programNumber <= 127; ++programNumber) {

        bool isPositive = programNumber >= 0;

        /**
         * Example:
         * programNumber = -103 = 0b10011001 (Two's Complement)
         * The (0b10 << 8) | programNumberConverted will not work. The result will still be 0b10011001 instead of 0b1010011001
         * Looks like the number must be 0b10011001, but positive to work the shifting.
         * That means 0b10011001 = 153
         * To keep the bits, but turn it into positive just move it to uint8_t. That way the shifting will work as expected.
         */
        uint8_t programNumberConverted = programNumber;

        int combination0Address = (0b00 << 8) | programNumberConverted;
        int combination1Address = (0b10 << 8) | programNumberConverted;
        int combination2Address = (0b01 << 8) | programNumberConverted;
        int combination3Address = (0b11 << 8) | programNumberConverted;

        int programNumberPositive = abs(programNumber);
        int onesDigit = programNumberPositive % 10;
        int secondDigit = (programNumberPositive / 10) % 10;
        int thirdDigit = (programNumberPositive / 100) % 10;

        setEEPROMAddressData(combination0Address, segmentDisplayMappingDigits[thirdDigit]);
        setEEPROMAddressData(combination1Address, segmentDisplayMappingDigits[secondDigit]);
        setEEPROMAddressData(combination2Address, segmentDisplayMappingDigits[onesDigit]);
        setEEPROMAddressData(combination3Address, isPositive ? segmentDisplayCharacterMapping[0] : segmentDisplayCharacterMapping[1]);
    }
}

void programEEPROM3BitsSegmentDecoder() {
    Serial.println("Programming EEPROM as decoder for 3 bit to display decoder.");
    setEEPROMAddressData(0, 0b01111110);
    setEEPROMAddressData(1, 0b00010010);
    setEEPROMAddressData(2, 0b10111100);
    setEEPROMAddressData(3, 0b10110110);
    setEEPROMAddressData(4, 0b11010010);
    setEEPROMAddressData(5, 0b11100110);
    setEEPROMAddressData(6, 0b11101110);
    setEEPROMAddressData(7, 0b00110010);
    Serial.println("Programmed EEPROM as decoder for 3 bit to display decoder.");

    //printEEPROMAddress(0, 7, BINARY);
}

/**
 * Will shift the bits to the 74HC595 shift register.
 * We are using the Arduino's prebuild shift out function, but it is limited to shifting only 8 bits at a time.
 * Thus we use it twice.
 * The prebuild function will place each of the provided bits on the SER_PIN and clock the SR_CLK pin, which will store them.
 * The shift register has two storages. Shift and Storage Register. The outputed bits from it are from the Storage Register.
 * After storing the 16 bits in the shift register we clock the RCLK_PIN, which moves the stored bits in the Shift Register to the Storage Register.
 */
void setShiftRegisterBits(const uint16_t& bits) {
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

    setShiftRegisterBits(shiftOutData);
}

uint8_t readEEPROMAddress(const uint16_t& address) {

    setEEPROMPins(address, true);

    return digitalReadBetween(EEPROM_IO_START_PIN, EEPROM_IO_END_PIN);
}

void printEEPROMAddressBinary(const uint16_t& address) {

    uint8_t addressData = readEEPROMAddress(address);

    bool dataBits[8] = {};

    for (int i = 0; i < 8; ++i)
        dataBits[i] = (0b1 << (7 - i)) & addressData;

    char printBuffer[34];
    sprintf(printBuffer, "Address: %d Value: %d %d %d %d %d %d %d %d", address, dataBits[0], dataBits[1], dataBits[2], dataBits[3], dataBits[4], dataBits[5], dataBits[6], dataBits[7]);
    Serial.println(printBuffer);
}

void printEEPROMAddressDecimal(const uint16_t& address) {
    uint8_t addressData = readEEPROMAddress(address);

    char printBuffer[20];
    sprintf(printBuffer, "Address: %d Value: %d", address, addressData);
    Serial.println(printBuffer);
}

void printEEPROMAddressHex(const uint16_t& address) {
    uint8_t addressData = readEEPROMAddress(address);

    char printBuffer[20];
    sprintf(printBuffer, "Address: %d Value: %X", address, addressData);
    Serial.println(printBuffer);
}


void printEEPROMAddress(const uint16_t& address, const uint8_t& format) {

    switch (format) {

        case BINARY:
            return printEEPROMAddressBinary(address);

        case DECIMAL:
            return printEEPROMAddressDecimal(address);

        case HEX:
            return printEEPROMAddressHex(address);
    }

}

void printEEPROMAddress(const uint16_t& from, const uint16_t& to, const uint8_t& format) {

    for (uint16_t address = from; address <= to; ++address) {
        printEEPROMAddress(address, format);
    }
}

void setEEPROMAddressData(const uint16_t& address, const uint8_t& data) {

    setEEPROMPins(address, false);

    digitalWriteBetween(EEPROM_IO_START_PIN, EEPROM_IO_END_PIN, data);

    clockPin(EEPROM_WE_PIN, LOW, 1, US);
    delay(10);
}

/*---------------- Utils ----------------*/

/**
 * Will read each pin between the given ones and from the bits construct a integer.
 * The function is extremely useful, when you have multiple I/O pins connected sequentially to arduino and you want to read bits from them.
 *
 * Example:
 *
 * startPin: 5
 * endPin: 12
 * Current Pin State: pin:5 (HIGH) pin: 6 (HIGH) pin: 7 (LOW) pin: 8 (LOW) pin: 9 (HIGH) pin: 10 (LOW) pin: 11 (HIGH) pin: 12 (LOW)
 *
 * Integer to be returned: 202 (11001010 in binary)
 */
unsigned int digitalReadBetween(const unsigned int& startPin, const unsigned int& endPin) {

    unsigned int data = 0;

    for (unsigned int pin = startPin; pin <= endPin; ++pin) {
        pinMode(pin, INPUT);

        bool readBit = digitalRead(pin);

        if (readBit)
            data = (data << 1) | 0b1;
        else
            data = data << 1;
    }

    return data;
}

/**
 * Will set each pin between the given ones to the bit, which is position relative to the value
 * The function is extremely useful, when you have multiple I/O pins connected sequentially to arduino and you want to set bits on them.
 *
 * Example:
 *
 * startPin: 5
 * endPin: 12
 * value: 0b11001010
 *
 * Result Pins State: pin:5 (HIGH) pin: 6 (HIGH) pin: 7 (LOW) pin: 8 (LOW) pin: 9 (HIGH) pin: 10 (LOW) pin: 11 (HIGH) pin: 12 (LOW)
 */
void digitalWriteBetween(const unsigned int& startPin, const unsigned int& endPin, const unsigned int& value) {

    unsigned int bitIndex = 0;

    for (unsigned int pin = endPin; pin >= startPin; --pin) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, ((0b1 << bitIndex) & value) > 0);
        bitIndex++;
    }
}

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
