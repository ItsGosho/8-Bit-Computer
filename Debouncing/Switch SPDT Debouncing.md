# **Switch SPDT Bouncing Problem:**

- Switch-a избира състояние и остава на него
- При SPDT Switch-a може да избираме между две състояниа на които да останем



Bouncing:

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220601025622266.png" alt="image-20220601025622266" style="zoom: 80%;" />

- Когато сменяме от A към B или обратното от B към А се получава същото рикоширане както при Button
- Тук обаче рикоширането **НЕ е м/у двете състояния** (**A -> B**, **B -> A**, **A -> B**, **B -> A** ... **A -> B**):

<img src="C:\Users\Gosho\AppData\Roaming\Typora\typora-user-images\image-20220601030339518.png" alt="image-20220601030339518" style="zoom:80%;" />





- Рикоширането **Е между новото състояние е междинно такова**:

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220601030655647.png" alt="image-20220601030655647" style="zoom:80%;" />

### Различни варианти за справяне с този проблем:

1. **SR Latch:**

- Ако използваме SR Latch, който два входа да са свързани към тези на SPDT Switch-a лесно си решаваме проблема.
- Това става така, защото когато сменим от едно състояние в друго и се получи Bouncing-a от новото състояние към междинното:
- - SR Latch изхода няма да се промени, колкото и пъти да се подава новия вход от нулев такъв след първоначалното



<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220601031841147.png" alt="image-20220601031841147" style="zoom:80%;" />

- Резисторите нарочно са там, а не само един към Ground, защото когато влезе в междинно състояние няма да имаме никакво съпротивление
- Вече SPDT Switch-a ни става дигитален.
