#include <Arduino.h>

//Pins on the Arduino Nano, that are connected to the corresponding Shift Register's Bits
#define SHIFT_REGISTER_D0_PIN 2
#define SHIFT_REGISTER_D1_PIN 3
#define SHIFT_REGISTER_D2_PIN 4
#define SHIFT_REGISTER_D3_PIN 5
#define SHIFT_REGISTER_D4_PIN 6
#define SHIFT_REGISTER_D5_PIN 7
#define SHIFT_REGISTER_D6_PIN 8
#define SHIFT_REGISTER_D7_PIN 9

#define SHIFT_REGISTER_SER_PIN 2
#define SHIFT_REGISTER_RCLK_PIN 3
#define SHIFT_REGISTER_SR_CLK_PIN 4

void setup() {
    Serial.begin(9600);
    Serial.println("EEPROM Start!");
    //pinMode(SHIFT_REGISTER_D0_PIN, INPUT);
    //pinMode(SHIFT_REGISTER_D1_PIN, INPUT);
    //pinMode(SHIFT_REGISTER_D2_PIN, INPUT);
    //pinMode(SHIFT_REGISTER_D3_PIN, INPUT);
    //pinMode(SHIFT_REGISTER_D4_PIN, INPUT);
    //pinMode(SHIFT_REGISTER_D5_PIN, INPUT);
    //pinMode(SHIFT_REGISTER_D6_PIN, INPUT);
    //pinMode(SHIFT_REGISTER_D7_PIN, INPUT);

    pinMode(SHIFT_REGISTER_SER_PIN, OUTPUT);
    //pinMode(SHIFT_REGISTER_OE_PIN, OUTPUT);
    pinMode(SHIFT_REGISTER_RCLK_PIN, OUTPUT);
    pinMode(SHIFT_REGISTER_SR_CLK_PIN, OUTPUT);
}

void printRegisterData() {
    char printBuffer[16];

    bool bit0 = digitalRead(SHIFT_REGISTER_D0_PIN);
    bool bit1 = digitalRead(SHIFT_REGISTER_D1_PIN);
    bool bit2 = digitalRead(SHIFT_REGISTER_D2_PIN);
    bool bit3 = digitalRead(SHIFT_REGISTER_D3_PIN);
    bool bit4 = digitalRead(SHIFT_REGISTER_D4_PIN);
    bool bit5 = digitalRead(SHIFT_REGISTER_D5_PIN);
    bool bit6 = digitalRead(SHIFT_REGISTER_D6_PIN);
    bool bit7 = digitalRead(SHIFT_REGISTER_D7_PIN);

    sprintf(printBuffer, "%d %d %d %d %d %d %d %d", bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7);
    Serial.println(printBuffer);
}

void writeBitToShiftRegister(const bool& bit) {

    digitalWrite(SHIFT_REGISTER_SER_PIN, bit);
    delayMicroseconds(1);

    digitalWrite(SHIFT_REGISTER_SR_CLK_PIN, HIGH);
    delayMicroseconds(1);
    digitalWrite(SHIFT_REGISTER_SR_CLK_PIN, LOW);

    delayMicroseconds(1);

    /*  digitalWrite(SHIFT_REGISTER_RCLK_PIN, HIGH);
      delayMicroseconds(1);
      digitalWrite(SHIFT_REGISTER_RCLK_PIN, LOW);

      delayMicroseconds(1);*/
}

void clearShiftRegister() {
    for (int i = 0; i < 8; ++i) {
        writeBitToShiftRegister(LOW);
    }
}

void test1() {
    //Fill with 0
    //delay(3000);

    writeBitToShiftRegister(HIGH);
    //printRegisterData();
    //delay(3000);

    writeBitToShiftRegister(HIGH);
    //printRegisterData();
    //delay(3000);

    writeBitToShiftRegister(LOW);
    //printRegisterData();
    //delay(3000);

    writeBitToShiftRegister(HIGH);
    printRegisterData();
    delay(1000);
}

void test(const int& address, const bool& outputEnable) {
    int shiftOutData = (outputEnable << 11) | address;

    int msb = shiftOutData >> 8;
    int lsb = shiftOutData & 0b0000000011111111;

    shiftOut(SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_SR_CLK_PIN, MSBFIRST, msb);
    shiftOut(SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_SR_CLK_PIN, MSBFIRST, lsb);

    digitalWrite(SHIFT_REGISTER_RCLK_PIN, HIGH);
    digitalWrite(SHIFT_REGISTER_RCLK_PIN, LOW);
}

void loop() {

    //test1();

    //clearShiftRegister();
/*    shiftOut(SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_SR_CLK_PIN, MSBFIRST, 0b111);
    shiftOut(SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_SR_CLK_PIN, MSBFIRST, 0b11111111);
    //shiftOut(SHIFT_REGISTER_SER_PIN, SHIFT_REGISTER_SR_CLK_PIN, MSBFIRST, 0b11010110);
    digitalWrite(SHIFT_REGISTER_RCLK_PIN, HIGH);
    digitalWrite(SHIFT_REGISTER_RCLK_PIN, LOW);*/

    for (int i = 1; i <= 2047; ++i) {
        test(i, true);
        delay(200);
    }


    //printRegisterData();
    delay(1000000);

    //shiftOut(SHIFT_REGISTER_SER_PIN,)

}