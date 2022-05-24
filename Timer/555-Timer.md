# 555 Timer:

- Генерира електрически сигнал през определено време.
- Има две състояния Charging (Output), Discharging (No output)



Според информацията, която намерих в интернет за неговите "вътрешности" направих опит да го направя в **[CircuitJS](https://www.falstad.com/circuit/circuitjs.html)**, но изникнаха проблеми и се наложи да го преправя.

![image-20220524052932738](C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220524052932738.png)

- Resistor 1 (1k) променя само Time High
- Resistor 2 (100k) променя Time High и Time Low
- Capacitor (1u) променя Time High и Time Low значително



1. Създаваме **Voltage Divider** с **3** бр. 5k резистора:

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220523024620290.png" alt="image-20220523024620290" style="zoom:80%;" />

- Идеята е да получим **различни нива на напрежение** м/у резисторите, който ще ни **служат за Comparator-ите после**.

2. Залагаме **2** бр. Comprator-и, които ще използваме за **Set & Reset** на бъдещият **SR Latch**.

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220523025426633.png" alt="image-20220523025426633" style="zoom:80%;" />

- **Comparator сравнява напрежения**. Ако това на **+ > -** , то Comparator-a ще се активира и ще пуска ток, докато това условие е валидно.
- В нашият случай:
- - *Reset Comparator + > 3.3V*
  - *Set Comparator 1.7V > -*

3. Залагаме **SR Latch**, който ще ни позволи да запазим **двете нужни състояния** на нашият **555 Timer**:

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220523030827557.png" alt="image-20220523030827557" style="zoom:80%;" />

- **SR Latch**-a ни е нужнен да **запомним** в което състояние се намираме - **Output** или **Discharge**
- При **Output** и **Discharge** периодите, **Reset** и **Set Comparator**-ите няма да бъдат активни. Затова използваме **SR Latch-a** да запомним **последното състояние** което е било избрано.

| Period    | Reset Comparator + Voltage | Set Comparator - Voltage |
| --------- | -------------------------- | ------------------------ |
| Output    | Спада от 3.3V до 1.7V      | Набира от 1.7V до 3.3V   |
| Discharge | Набира от 1.7V до 3.3V     | Спада от 3.3V до 1.7V    |

4.  Основната контролираща линия:

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220524044759284.png" alt="image-20220524044759284" style="zoom:80%;" />

- Заложи ли сме **2** бр. резистори в серия и **1** бр. кондензатор в серия с резисторите.
- Така ще **забавим времето за зареждане на кондензатора**, като това ще ни позволи **напрежението преди него да се качва по-бавно**.
- *Времето за качване на напрежението зависи от големината на кондензатора и съпротивлението в серия с него.*
- Note: Първият резистор (1K) е сложен нарочно, защото ще ни е нужен за Discharge логиката после.

5. Discharge логика:

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220524045551778.png" alt="image-20220524045551778" style="zoom:80%;" />

- Залагаме, когато имаме състояние на **Discharge** при **SR Latch-a** да активираме **NPN Transistor**.
- NPN Transistor-a при активиране ще свързва **основната контролираща линия** с Ground.
- Когато се отвори **Discharge** линията, то тока от пътя **5V -> R4 -> R5 -> Reset Comparator, Set Comparator и C1** няма да следва този път вече, защото този на **Discharge** линията е с **по-малко съпротивление**.  Съотвено новият му път е **(5V -> R4 -> NPN T1 -> Ground)**
- **R5, C1, Reset Comparator и Set Comparator** ще загубят напрежение заради новата линия **5V -> R4 -> NPN T1 -> Ground** и тогава ще се активира **C1**. Вече имаме нова линия **C1 -> R5, Reset Comparator, Set Comparator -> NPN T1 -> Ground**. Сега постепенно напрежението на Reset Comparator и Set Comparator ще спада, докато отново не се активира Set Comparator-a. Тогава целият цикъл започва отначало.



**Charging Flow:**

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220524052612851.png" alt="image-20220524052612851" style="zoom:50%;" />

**Discharging Flow:**

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220524052829054.png" alt="image-20220524052829054" style="zoom:50%;" />

https://www.digikey.bg/en/resources/conversion-calculators/conversion-calculator-555-timer
