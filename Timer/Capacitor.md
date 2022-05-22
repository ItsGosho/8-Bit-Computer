Калкулатор за voltage discharge за определено време:

http://www.learningaboutelectronics.com/Articles/Capacitor-discharge-calculator.php#answer

Калкулатор за voltage discharge общо нужно време:

https://www.digikey.bg/en/resources/conversion-calculators/conversion-calculator-capacitor-safety-discharge



Charge time calculator:

https://www.allaboutcircuits.com/tools/capacitor-Charge-and-time-constant-calculator/



https://www.arduino.cc/documents/datasheets/cap100uF.pdf



https://electronics.stackexchange.com/questions/619464/why-capacitor-discharge-time-is-higher-when-a-led-is-included

TODO: Да си вкарам тестовете със LED-a.



Discharging:



В зависимост



------

Кондензатора е компонент, който съхранява електрическа енергия до момента в който напрежението на електрическата верига в която се намира не започне да пада. Тогава той започва да отделя съхранената си енергия мигновенно докато не се изравни с напрежението отново.

По този начин ако имаме негативна промяна във напрежението, кондензатора ще направи спадането на напрежението по-плавно.



##### Тест: Проста eл. верига, която се пуска след 1 секунда, а след още 3 секунди се спира. **Съдържа един кондензатор и резистор**:

**Без кондензатор:**

- Falling Time-a е доста по-бърз

  <img src="C:\Users\Gosho\AppData\Roaming\Typora\typora-user-images\image-20220515201011705.png" alt="image-20220515201011705" style="zoom: 67%;" />



**Със кондензаор:**

- Falling Time-a е доста по-бавен.

  <img src="C:\Users\Gosho\AppData\Roaming\Typora\typora-user-images\image-20220515202044540.png" alt="image-20220515202044540" style="zoom: 67%;" />



- Напрежението никога не се покачва/спада изведнъж.

- Просто на горните две се гледа графиката в секунди, но ако я разгледаме в по-ниска единица (приближено) това се забелязва.

Долните две снимки са пример на електрическата верига без кондензатор.

<img src="C:\Users\Gosho\AppData\Roaming\Typora\typora-user-images\image-20220515202511956.png" alt="image-20220515202511956" style="zoom:67%;" />

<img src="C:\Users\Gosho\AppData\Roaming\Typora\typora-user-images\image-20220515202546995.png" alt="image-20220515202546995" style="zoom: 67%;" />



- Това за колко време ще се зареди кондензатора и изтощи при спиране на тока зависи от големината на кондезатора и съпротивлението във веригата.
- Може да използваме калкулатор за да изчислим времето за 65% разряд на кондезатора. Това ще рече, че ако имаме пр. 5V, 100 mf, 10 kiloohms XXX
- Може да използваме калкулатор за да изчислим времето за зареждане на кондензатора, като той е свързан в серия със резистора:
- Снимка +++
- https://www.allaboutcircuits.com/tools/capacitor-Charge-and-time-constant-calculator/







