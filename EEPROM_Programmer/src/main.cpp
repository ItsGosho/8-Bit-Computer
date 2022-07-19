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

#define SHIFT_REGISTER_SER_PIN 10
#define SHIFT_REGISTER_OE_PIN 11
#define SHIFT_REGISTER_RCLK_PIN 12
#define SHIFT_REGISTER_SR_CLK_PIN 13

void setup() {
    Serial.begin(9600);
    Serial.println("EEPROM Start!");
    pinMode(SHIFT_REGISTER_D0_PIN, INPUT);
    pinMode(SHIFT_REGISTER_D1_PIN, INPUT);
    pinMode(SHIFT_REGISTER_D2_PIN, INPUT);
    pinMode(SHIFT_REGISTER_D3_PIN, INPUT);
    pinMode(SHIFT_REGISTER_D4_PIN, INPUT);
    pinMode(SHIFT_REGISTER_D5_PIN, INPUT);
    pinMode(SHIFT_REGISTER_D6_PIN, INPUT);
    pinMode(SHIFT_REGISTER_D7_PIN, INPUT);

    pinMode(SHIFT_REGISTER_SER_PIN, OUTPUT);
    pinMode(SHIFT_REGISTER_OE_PIN, OUTPUT);
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

void loop() {

    digitalWrite(SHIFT_REGISTER_SER_PIN, HIGH);
    delayMicroseconds(1);

    digitalWrite(SHIFT_REGISTER_SR_CLK_PIN, HIGH);
    delayMicroseconds(1);
    digitalWrite(SHIFT_REGISTER_SR_CLK_PIN, LOW);

    delayMicroseconds(1);

    digitalWrite(SHIFT_REGISTER_RCLK_PIN, HIGH);
    delayMicroseconds(1);
    digitalWrite(SHIFT_REGISTER_RCLK_PIN, LOW);

    delayMicroseconds(1);

    printRegisterData();
    delay(10000);
}