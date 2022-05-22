# D Latch:

- **D Latch** е акроним на **Data Latch**

- То е **Multivibrator** от тип **Bistable**

- Реализира се с **SR Latch**, 2 бр. **AND Gate**, **Inverter**.

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220522030950296.png" alt="image-20220522030950296" style="zoom: 67%;" />

**D Latch** в [CircuitJS](https://www.falstad.com/circuit/circuitjs.html)

- Чрез експериментиране може да видим какво се случва във веригата
- Файл: **/CircuitJS/d_latch.circuitjs.txt**

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220522031244733.png" alt="image-20220522031244733" style="zoom: 67%;" />

- Той **надгражда SR Latch**, като позволява да определяме **дали може да се сменя състоянието на Latch-a чрез EN** и смяната на състоянието да става чрез **един вход** **D**
- Отново **не знаем** **началното** **състоянието** на **SR Latch**, ако **EN не е включен**.
- **Винаги** при подаването на **EN** без **D**,  Latch-a ще премине в **Reset (R)** състояние **заради Inverter-a**, който **постоянно подава сигнал**.
- Как би **изглеждал изхода** от **D Latch-a** при **примерно** подаване на сигнали:
- - **[DrawIo](https://app.diagrams.net/)** Timing Diagram
  - Файл: **/DrawIo/D_Latch_Timing_Diagram.drawio**

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220522040424845.png" alt="image-20220522040424845" style="zoom: 80%;" />

- Както виждаме от диаграмата, *изхода* на **Q** *зависи* от D, което *зависи* от **EN**. 

- Когато **EN** е включен, то **Q** *следва* **D** *1:1*, ако **EN** бъде **изключен** то **Q** остава **последното състояние** на **D**.
