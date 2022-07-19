# JK Flip Flop:

### Какво е JK-Flip-Flop:

- Идеята му е да реши проблема на **SR Latch with Enable**.
- Проблема, който **SR Latch with Enable** има е че ако **S (Set)** и **R (Reset)** са **HIGH** и **EN (Enable)** също е **HIGH**, то резултатите **Q** и **Q Inverted** ще са **LOW**.

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220703175859907.png" alt="image-20220703175859907" style="zoom:50%;" />

- Ако S (Set) и R (Reset) са HIGH, но все още EN (Enable) е LOW:

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220703180420797.png" alt="image-20220703180420797" style="zoom:50%;" />

- Ако искаме сега след като дойде EN (Enable) да нямаме Q и Q Inverted - LOW, то ако сложим AND за да може:
  - R да се активира, ако Q е HIGH
  - S да се активира, ако Q Inverted е HIGH
- Така ще избегнем проблема Q и Q Inverted да са LOW, когагато EN (Enable) е HIGH

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220703182831464.png" alt="image-20220703182831464" style="zoom:50%;" />

- Обаче сега има друг проблем, подава се EN (Enable) - HIGH и S (Set) и R (Reset) - HIGH, когато се сменя състоянието към противоположното, то ще се смени отново обратно и същият този цикъл може да се повтори няколко пъти без да сме сигурни в какво състояние ще завършим.
- Искаме, когато EN (Enable) е HIGH, S (Set) и R (Reset) са HIGH Latch-a да се сменя в противоположното състояние винаги.



### Какво е Master-Slave JK-Flip-Flop:

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220703185232962.png" alt="image-20220703185232962" style="zoom:50%;" />



- Както виждаме наследява 2-та AND-та, които предотвратяват LOW на изходите и пускане само на противоположното.
- Тук обаче имаме SR Latch-a, който играе ролята на посредник (Master) между финалният ни SR Latch (Slave).
- Сега, когато дойде EN (Clock) ни е HIGH, няма все още да сме подали противоположната стойност към финалният SR Latch (Slave).

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220703185806568.png" alt="image-20220703185806568" style="zoom:50%;" />



<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220703185816063.png" alt="image-20220703185816063" style="zoom:50%;" />

- Както виждаме тя отива в първият SR Latch (Master), но все още не се пуска към финалният (Slave), защото трябва да отпуснем EN (Clock) за да стане това.
- Така Feedback-a от финалният SR Latch (Slave) не прецаква AND-овете в началото, защото за да стигнат до края и отново да си подадат новият резултат трябва да отпуснем Clock-a, а след като е отпуснат няма да може да приеме новите резултати от Feedback-a и да ги подаде отново.
