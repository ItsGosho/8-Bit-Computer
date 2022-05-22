# SR Latch:

- SR Latch е акроним на **Set-Reset Latch**
- То е **Multivibrator** от тип **Bistable**
- Реализира се със **два** **NOR Gate**, които са **свързани взаимно**.



<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220521213112131.png" alt="image-20220521213112131" style="zoom:33%;" />



**SR Latch** в [CircuitJS](https://www.falstad.com/circuit/circuitjs.html):

- Чрез експериментиране може да видим какво се случва във веригата
- Файл: **/CircuitJS/sr_latch.circuitjs.txt**



<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220520173601137.png" alt="image-20220520173601137" style="zoom: 67%;" />



- Приемаме, че **Reset (R)** е отгоре, а **Set (S)** е отдолу, съответно **Q** е отгоре, а **Q Inverted** отдолу.
- Ако **Reset (R)** и **Set (S)** са **On** едновременно, то имаме **невалидно състояние**.
- Ако след избирането на състояние чрез **подаване** на **Reset (R)** или **Set (S)** **премахнем** подаденото състояние, то **остава**. (*От тук идва смисъла на Latch-a*). Това **също означава**, че **сегашното състояние** на **Q** или **Q Inverted** е последното избрано от **Reset (R)** или **Set (S)**

- Състоянието в което ще стартира е винаги на случаен принцип, защото според променливи фактори единият сигнал ще достигне по-бързо от другият.
- Друго обяснение на [Race Condition](https://electronics.stackexchange.com/questions/155949/what-is-race-condition-in-flip-flops)



<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220521214556400.png" alt="image-20220521214556400" style="zoom:67%;" />



- След като SR Latch-a е стартирал в дадено състояние, то ще го пази.

- Не се губи, защото постоянно се подава м/у NOR Gate-овете.

- Като променяме входа на **Reset (R)** и **Set (S)** можем да променим състоянието на **SR Latch** */* **Q** и **Q Inverted**

  | Reset (R) | Set (S) | Q    | Q Inverted |
  | --------- | ------- | ---- | ---------- |
  | 0         | 0       | Q    | Q          |
  | 0         | 1       | 1    | 0          |
  | 1         | 0       | 0    | 1          |
  | 1         | 1       | 0    | 0          |

  

- Като стойности са сложени **Q** или **Q Inverted**, защото или получава същата стойност или обратната.