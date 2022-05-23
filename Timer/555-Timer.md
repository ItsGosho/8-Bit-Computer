https://www.youtube.com/watch?v=i0SNb__dkYI







Направих си 555 Timer от нулата.

TODO:



# 555 Timer:

- Генерира електрически сигнал през определено време.



Според информацията, която намерих в интернет за неговите "вътрешности" направих опит да го направя в CircuitJS, но изникнаха проблеми и се наложи да го преправя.

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220522150429415.png" alt="image-20220522150429415" style="zoom:80%;" />

TODO: 

- Да маркирам в CircuitJS Pin-овете

- Как работи от врътре?

- Resistor 1 (1k) променя само Time High
- Resistor 2 (100k) променя Time High и Time Low
- Capacitor (1u) променя Time High и Time Low значително



1. Създаваме Voltage Divider с 3 бр. 5k резистора:

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220523024620290.png" alt="image-20220523024620290" style="zoom:80%;" />

- Идеята е да получим различни нива на напрежение м/у резисторите, който ще ни служат за Comparator-ите после.

2. Залагаме 2 бр. Comprator-и, които ще използваме за Set & Reset на бъдещият SR Latch.

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220523025426633.png" alt="image-20220523025426633" style="zoom:80%;" />

- Comparator сравнява напреженията. Ако това на + > - , то Comparator-a ще се активира и ще пуска ток
- В нашият случай:
- - Reset Comparator + > 3.3V
  - Set Comparator 1.7V > -

3. Залагаме SR Latch, който ще ни позволи да запазим двете нужни състояния на нашият 555 Timer:

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220523030827557.png" alt="image-20220523030827557" style="zoom:80%;" />

- SR Latch-a ни е нужнен да запомни в което състояние се намираме Output или Discharge
- При Charge и Discharge периодите, Reset и Set Comparator-ите няма да бъдат активни:

| Period    | Reset Comparator + Voltage | Set Comparator - Voltage |
| --------- | -------------------------- | ------------------------ |
| Charge    | Спада от 3.3V до 1.7V      | Набира от 1.7V до 3.3V   |
| Discharge | Набира от 1.7V до 3.3V     | Спада от 3.3V до 1.7V    |





https://www.digikey.bg/en/resources/conversion-calculators/conversion-calculator-555-timer
