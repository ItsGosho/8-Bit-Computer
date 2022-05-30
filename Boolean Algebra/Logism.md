# Logism:

- Много добра програма за създаване на елктрически схеми от логически елементи според израз или таблица



1. **Създаване на електрическа схема за булева таблица:**

- Ако приемем, че имаме следната булева таблица:

| S (Select) | A    | B    | O    |
| ---------- | ---- | ---- | ---- |
| 0          | 0    | 0    | 0    |
| 0          | 0    | 1    | 0    |
| 0          | 1    | 0    | 1    |
| 0          | 1    | 1    | 1    |
| 1          | 0    | 0    | 0    |
| 1          | 0    | 1    | 1    |
| 1          | 1    | 0    | 0    |
| 1          | 1    | 1    | 1    |

- Отиваме при **Window** -> **Combinational Analysis**
- **Inputs** добавяме всичките входове - **S**, **A** и **B**
- **Outputs** добавяме всичките изходи - **O**
- **Table** попълваме с кликане върху всеки ред за **O**, какво очакваме при комбинацията от **S**, **A** и **B**

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220531014955532.png" alt="image-20220531014955532" style="zoom: 67%;" />

- **Build Circuit** и сме готови.
- [**Note**] Чрез този метод автоматично получаваме булевият израз (опростен) в **Expression**

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220531020215086.png" alt="image-20220531020215086" style="zoom: 80%;" />

2. **Създаване на електрическа схема за булев израз:**

- Ако прием, че имаме следният булев израз:

- $$
  \\ \bar{S}.A.\bar{B} +
  \bar{S}.A.B +
  S.\bar{A}.B + 
  S.A.B =
  O
  $$

- [**Note**] Създаден е от горната булева таблица, **но не е опростен**.

- Отиваме при **Window** -> **Combinational Analysis**

- **Inputs** добавяме всичките входове - **S**, **A** и **B**

- **Outputs** добавяме всичките изходи - **O**

- **Expression** попълваме булевият, даваме **Enter ** и **Build Circuit**:

- `~S A ~B + ~S A B + S ~A B + S A B`

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220531015718917.png" alt="image-20220531015718917" style="zoom: 80%;" />



<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220531020238361.png" alt="image-20220531020238361" style="zoom:80%;" />
