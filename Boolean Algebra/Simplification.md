`+` = OR

`.` = AND



![image-20220531022944499](C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220531022944499.png)

- A.A = A, защото A е една и съща стойност.
- - Тоест е променлива, която няма едновременно как да има две различни стойности
  - `A = 1, 1.1 = 1`. `A = 0, 0.0 = 0`

- A.0 = 0, защото `.` репрезентира AND, съответно 0 със каквото и да е било при AND Gate ще даде 0-ла.



<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220531032816217.png" alt="image-20220531032816217" style="zoom:50%;" />

- Изглежда най-добрият вариант са такива създадени от таблица е да използваме директно Distributive и после елиминиране.

- Сходната логика за такива изрази от таблица се получава защото винаги генерираме в един и същи формат:

- - X..**r** + n
  -  **r** са колоните от таблицата взети за генерирането на изразите съставящи Output-a
  - n са редовете от таблицата взети за генерирането на изразите, където изхода е 1

  - Примерно ако сме имали таблица с 3 входа (A, B, C), и 5 реда, където имаме 1-ца:
  - - ABC + ABC + ABC + ABC + ABC, като само не знаем къде имам обърнати стойности на A, B или C, съставящи дадената 1-ца

  