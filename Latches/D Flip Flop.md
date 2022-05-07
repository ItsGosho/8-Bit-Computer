# D Flip Flop

- Представлява D Latch, който бива активиран от Clock вместо директно от бутон.
- При Clock-a отново имаме подаване на сигнал, но е само един път за да сложим състоянието на Set/Reset-a.

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220507203511679.png" alt="image-20220507203511679"  />

| D Latch                                                      | D Flip Flop                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| 1. Задържаме EN бутона. <br>2. Задържаме/Отпускаме D бутона за да изберем състояние на Latch-a.<br>3. Отпускаме EN бутона, докато все още сме задържали/отпуснали D бутона за да сложим състоянието.<br> | 1. Задържаме/Отпускаме D бутона за да изберем състояние на Latch-a.<br>2. Натискаме CLK бутона докато сме задържали/отпуснали D бутона за да сложим състоянието. |
| Има нужда Enable логиката (EN) да бъде задържана за да може да се променя състоянието. Трябва да бъде задържана, след това избрано състояние и после отпусната без да се отпуска D. | Няма нужда Enable логиката (CLK) да бъда задържана за да може да се променя състоянието. Нужно е само един път да се натисне и се слага мигновенно състоянието. |
| Ако оставим Enable логиката може да променяме състоянието.   | Дори и да оставим Enable логиката натисната няма повече да се промени състоянието. Това става мигновенно при първоначалното натискане. |
