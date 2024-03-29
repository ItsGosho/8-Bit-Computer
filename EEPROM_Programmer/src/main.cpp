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
 * Because, we have many control signals we can't use a single EEPROM.
 * ! Single EEPROM have only 8 outputs, which can't cover all signals and thus we can't active them.
 * That means we must use 2 EEPROMs and program them separately. For the same address program first half and then the second.
 * For easier programming the eeproms, instead of each time making the bits for a given microinstruction we can pre-define where the bits for a given
 * control signal to be activated need to be and the using bitwise operators create the final control signals for single microinstruction.
 */

//First EEPROM
#define HLT_CS 0b10000000
#define MI_CS 0b01000000
#define RI_CS 0b00100000
#define RO_CS 0b00010000
#define IO_CS 0b00001000
#define II_CS 0b00000100
#define AI_CS 0b00000010
#define AO_CS 0b00000001

//Second EEPROM
#define EO_CS  0b10000000
#define SU_CS  0b01000000
#define BI_CS  0b00100000
#define OI_CS  0b00010000
#define CE_CS  0b00001000
#define CO_CS  0b00000100
#define J_CS   0b00000010

const uint8_t LDA_INSTRUCTION_CODE = 0b0000;
const uint8_t LDB_INSTRUCTION_CODE = 0b0001;
const uint8_t ADD_INSTRUCTION_CODE = 0b0010;
const uint8_t SUB_INSTRUCTION_CODE = 0b0011;
const uint8_t STA_INSTRUCTION_CODE = 0b0100;
const uint8_t LDI_INSTRUCTION_CODE = 0b0101;
const uint8_t JMP_INSTRUCTION_CODE = 0b0110;
const uint8_t OUT_INSTRUCTION_CODE = 0b0111;
const uint8_t HLT_INSTRUCTION_CODE = 0b1000;

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

void setShiftRegisterBits(const uint16_t& bits, const uint8_t& bitOrder = MSBFIRST);

void setEEPROMPins(const uint16_t& address, const bool& outputEnable, const uint8_t& bitOrder = MSBFIRST);

uint8_t readEEPROMAddress(const uint16_t& address);

void printEEPROMAddressBinary(const uint16_t& address);

void printEEPROMAddressDecimal(const uint16_t& address);

void printEEPROMAddressHex(const uint16_t& address);

void printEEPROMAddress(const uint16_t& address, const uint8_t& format);

void printEEPROMAddress(const uint16_t& from, const uint16_t& to, const uint8_t& format);

void setEEPROMAddressData(const uint16_t& address, const uint8_t& data, const uint8_t& bitOrder = MSBFIRST);

void programEEPROM3BitsSegmentDecoder();

void programEEPROM8BitsSegmentDecoder();

uint16_t generateMicroinstructionAddress(const uint8_t& step, const uint8_t& instruction);

/*
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
 * Instruction | N Microinstruction | Activate Control Signals (HLT, MI, RI, RO, IO ... J)
 *
 * 0001             000                 0100000000000010
 * 0001             001                 0001010000000100
 * 0001             010                 0100100000000000
 * 0001             011                 0001001000000000
 *
 * 0b00000{Microinstruction 3 bits}{Instruction 4 Bits}
 * 0b00000{Step 3 bits}{Code 4 Bits}
 * */
void programFirstEEPROM() {

    setEEPROMAddressData(generateMicroinstructionAddress(0, LDA_INSTRUCTION_CODE), MI_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, LDA_INSTRUCTION_CODE), RO_CS | II_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, LDA_INSTRUCTION_CODE), MI_CS | IO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(3, LDA_INSTRUCTION_CODE), RO_CS | AI_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(4, LDA_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, LDB_INSTRUCTION_CODE), MI_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, LDB_INSTRUCTION_CODE), RO_CS | II_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, LDB_INSTRUCTION_CODE), MI_CS | IO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(3, LDB_INSTRUCTION_CODE), RO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(4, LDB_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, ADD_INSTRUCTION_CODE), MI_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, ADD_INSTRUCTION_CODE), RO_CS | II_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, ADD_INSTRUCTION_CODE), MI_CS | IO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(3, ADD_INSTRUCTION_CODE), RI_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(4, ADD_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, SUB_INSTRUCTION_CODE), MI_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, SUB_INSTRUCTION_CODE), RO_CS | II_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, SUB_INSTRUCTION_CODE), MI_CS | IO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(3, SUB_INSTRUCTION_CODE), RO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(4, SUB_INSTRUCTION_CODE), AI_CS);

    setEEPROMAddressData(generateMicroinstructionAddress(0, STA_INSTRUCTION_CODE), MI_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, STA_INSTRUCTION_CODE), RO_CS | II_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, STA_INSTRUCTION_CODE), MI_CS | IO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(3, STA_INSTRUCTION_CODE), RI_CS | AO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(4, STA_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, LDI_INSTRUCTION_CODE), MI_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, LDI_INSTRUCTION_CODE), RO_CS | II_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, LDI_INSTRUCTION_CODE), IO_CS | AI_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(3, LDI_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(4, LDI_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, JMP_INSTRUCTION_CODE), MI_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, JMP_INSTRUCTION_CODE), RO_CS | II_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, JMP_INSTRUCTION_CODE), IO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(3, JMP_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(4, JMP_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, OUT_INSTRUCTION_CODE), MI_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, OUT_INSTRUCTION_CODE), RO_CS | II_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, OUT_INSTRUCTION_CODE), AO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(3, OUT_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(4, OUT_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, HLT_INSTRUCTION_CODE), MI_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, HLT_INSTRUCTION_CODE), RO_CS | II_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, HLT_INSTRUCTION_CODE), HLT_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(3, HLT_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(4, HLT_INSTRUCTION_CODE), 0);
}

void programSecondEEPROM() {

    setEEPROMAddressData(generateMicroinstructionAddress(0, LDA_INSTRUCTION_CODE), CO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, LDA_INSTRUCTION_CODE), CE_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, LDA_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(3, LDA_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(4, LDA_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, LDB_INSTRUCTION_CODE), CO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, LDB_INSTRUCTION_CODE), CE_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, LDB_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(3, LDB_INSTRUCTION_CODE), BI_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(4, LDB_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, ADD_INSTRUCTION_CODE), CO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, ADD_INSTRUCTION_CODE), CE_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, ADD_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(3, ADD_INSTRUCTION_CODE), EO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(4, ADD_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, SUB_INSTRUCTION_CODE), CO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, SUB_INSTRUCTION_CODE), CE_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, SUB_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(3, SUB_INSTRUCTION_CODE), EO_CS | SU_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(4, SUB_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, STA_INSTRUCTION_CODE), CO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, STA_INSTRUCTION_CODE), CE_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, STA_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(3, STA_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(4, STA_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, LDI_INSTRUCTION_CODE), CO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, LDI_INSTRUCTION_CODE), CE_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, LDI_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(3, LDI_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(4, LDI_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, JMP_INSTRUCTION_CODE), CO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, JMP_INSTRUCTION_CODE), CE_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, JMP_INSTRUCTION_CODE), J_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(3, JMP_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(4, JMP_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, OUT_INSTRUCTION_CODE), CO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, OUT_INSTRUCTION_CODE), CE_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, OUT_INSTRUCTION_CODE), OI_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(3, OUT_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(4, OUT_INSTRUCTION_CODE), 0);

    setEEPROMAddressData(generateMicroinstructionAddress(0, HLT_INSTRUCTION_CODE), CO_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(1, HLT_INSTRUCTION_CODE), CE_CS);
    setEEPROMAddressData(generateMicroinstructionAddress(2, HLT_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(3, HLT_INSTRUCTION_CODE), 0);
    setEEPROMAddressData(generateMicroinstructionAddress(4, HLT_INSTRUCTION_CODE), 0);
}

void setup() {
    Serial.begin(BAUD_RATE);
    Serial.println("EEPROM Start!");

    digitalWrite(EEPROM_WE_PIN, HIGH);

    int outputPins[4] = {EEPROM_WE_PIN, SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_RCLK_PIN, SHIFT_REGISTER_SR_CLK_PIN};
    pinModes(outputPins, OUTPUT);



    Serial.println("Started programming!");
    //programFirstEEPROM();
    programSecondEEPROM();
    Serial.println("Finished programming!");
}

void loop() {

}

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
 * Example:
 * Let's say we have the OUT instruction.
 * The OUT instruction unique code (4 bit) is 0110.
 * The OUT instruction needs 5 total microinstruction.
 *       EEPROM Addr. |  Control Signals
 * 0:  0b000000000110      0b01000000
 * 1:  0b000000010110      0b00010100
 * 2:  0b000000100110      0b00000001
 * 3:  0b000000110110      0b00000000
 * 4:  0b000001000110      0b00000000
 *
 * Where EEPROM Address's patter is the following:
 * 0b00000{N Microinstruction (3 bits)}{Instruction (4 Bits)}
 *
 * Instead of programming the eeprom the hard way and getting lost while writing the bits for each instruction's microinstructions:
 *  setEEPROMAddressData(0b000000000110, 0b01000000); //Microinstruction: 1;
 *  setEEPROMAddressData(0b000000010110, 0b00010100); //Microinstruction: 2;
 *  setEEPROMAddressData(0b000000100110, 0b00000001); //Microinstruction: 3;
 *  setEEPROMAddressData(0b000000110110, 0b00000000); //Microinstruction: 4;
 *  setEEPROMAddressData(0b000001000110, 0b00000000); //Microinstruction: 5;
 * You can use that method to easily generate them and not get lost in the bits:
 *  setEEPROMAddressData(generateMicroinstructionAddress(0, OUT_INSTRUCTION_CODE), 0b01000000); //Microinstruction: 1;
 *  setEEPROMAddressData(generateMicroinstructionAddress(1, OUT_INSTRUCTION_CODE), 0b00010100); //Microinstruction: 2;
 *  setEEPROMAddressData(generateMicroinstructionAddress(2, OUT_INSTRUCTION_CODE), 0b00000001); //Microinstruction: 3;
 *  setEEPROMAddressData(generateMicroinstructionAddress(3, OUT_INSTRUCTION_CODE), 0b00000000); //Microinstruction: 4;
 *  setEEPROMAddressData(generateMicroinstructionAddress(4, OUT_INSTRUCTION_CODE), 0b00000000); //Microinstruction: 5;
 *
 * @param step Which line of instruction is that.
 * @param code The unique code of the instruction
 */
uint16_t generateMicroinstructionAddress(const uint8_t& step, const uint8_t& code) {
    return 0b000000000000 | (step << 4) | code;;
}

/**
 * Will shift the bits to the 74HC595 shift register.
 * We are using the Arduino's prebuild shift out function, but it is limited to shifting only 8 bits at a time.
 * Thus we use it twice.
 * The prebuild function will place each of the provided bits on the SER_PIN and clock the SR_CLK pin, which will store them.
 * The shift register has two storages. Shift and Storage Register. The outputed bits from it are from the Storage Register.
 * After storing the 16 bits in the shift register we clock the RCLK_PIN, which moves the stored bits in the Shift Register to the Storage Register.
 */
void setShiftRegisterBits(const uint16_t& bits, const uint8_t& bitOrder) {
    uint8_t msb = bits >> 8;
    uint8_t lsb = bits & 0b11111111;

    shiftOut(SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_SR_CLK_PIN, bitOrder, msb);
    shiftOut(SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_SR_CLK_PIN, bitOrder, lsb);

    clockPin(SHIFT_REGISTER_RCLK_PIN, HIGH);
}

/**
 * Sets the address of the EEPROM and if the EEPROM's output will be enabled.
 */
void setEEPROMPins(const uint16_t& address, const bool& outputEnable, const uint8_t& bitOrder) {
    uint16_t shiftOutData = (!outputEnable << 11) | address;

    setShiftRegisterBits(shiftOutData, bitOrder);
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

void setEEPROMAddressData(const uint16_t& address, const uint8_t& data, const uint8_t& bitOrder) {

    setEEPROMPins(address, false, bitOrder);

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
