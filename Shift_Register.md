# Shift Register:

- SN74HC595N



- Shift Register (SN74HC595N) е от типа SIPO.
- SIPO - Serial In - Parallel Out:
  - Запазването на информацията в Shift Register-a става последователно (SIPO). Има и такива, при които е паралелно (PISO).
  - Извличането на запаената информация от Shift Register-а става наведнъж (SIPO). Има и такива, при които е последователно (PISO).



- Отдолу SIPO Shift Register-а е реализиран с няколко JK Flip Flop-a:

<img src="C:\Users\itsgo\AppData\Roaming\Typora\typora-user-images\image-20220719162906898.png" alt="image-20220719162906898" style="zoom:50%;" />

- Когато се подготви бит (DATA) и се вкара с Clock-a (CLK), то другите се изместват нагоре.
- Така може да подготвим множество данни, които да ги изкараме наведнъж.
- Примерно, ако използваме Arduino и не ни стигат пиновете да контролираме нещо, което иска да получи данните наведнъж на всичките си пинове, то може да ги подготвим в Shift Register.
- Тоест Shift Register-a ни се явява и като Converter между Serial и Parallel Communication.



### Връщаме се на SN74HC595N:

| Pin            | State       | Description                                                  |
| -------------- | ----------- | ------------------------------------------------------------ |
| Q0-Q7          | Output      | Това са пиновете с резултатите от Shift Register-a. Може да са в Hi-Z / 3-State състояние, ако OE е LOW - тоест изключение. |
| DS / SER       | Input       | Бита, който ще бъде запазен при Rising Edge-a на SRCLK       |
| OE             | Active Low  | Дали да се изкарват данните от Shift Register-a на Q0 - Q7, ако ли не то тези пинове са в Hi-Z / 3-State състояние. |
| ST_CP / RCLK   | Active High | Най-вероятно има два Storage-а вътре. Shift Register и Storage Register. Първо запазваме данните в Shift Register чрез DS / SER & SH_CP / SRCLK и след това подаваме пулс тук и те се местят в Storage Register, като тогава вече може да ги изкраме на изхода чрез OE. |
| SH_CP  / SRCLK | Active High | Запазва бита Rising Edge-а на подадения сигнал , който е подготвен на DS / SER пина. |
| MR / SRCLR     | Active Low  | Изчиства данните от регистъра.                               |

- Active High - Трябва да е подаден ток за да се изпълни действието, което извършва пина.
- Active Low - Трябва да НЕ е подаден ток за да се изпълни действието, което извършва пина.