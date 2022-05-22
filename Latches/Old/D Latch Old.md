

### Тест 1: (Enable Logic Only without SR Latch)

- Идеята на Enable e че натискаме бутон EN, който позволява бутон D да работи.
- D бутона в конкретният тест активира или Output 1 (O1) или Output 2 (O2).



<img src="..\Pictures\image-20220502202819906.png" alt="image-20220502202819906" style="zoom: 67%;" />

| D    | EN   | Output 1 | Output 2 |
| ---- | ---- | -------- | -------- |
| 0    | 0    | 0        | 0        |
| 0    | 1    | 1        | 0        |
| 1    | 0    | 0        | 0        |
| 1    | 1    | 0        | 1        |



### Тест 2: (Целият D-Latch)

<img src="..\Pictures\image-20220502214302615.png" alt="image-20220502214302615"  />

| EN   | D    | Ouput 1 | Output 2 | Output 3 | Output 4 |
| ---- | ---- | ------- | -------- | -------- | -------- |
| 0    | 0    | 0       | 0        | Output 3 | Output 4 |
| 1    | 0    | 1       | 0        | 1        | 0        |
| 0    | 1    | 0       | 0        | Output 3 | Output 4 |
| 1    | 1    | 0       | 1        | 1        | 0        |

Базиране е таблицата върху **Hold EN** -> **Hold/Realese D** -> **Realese EN**.

<img src="..\Pictures\image-20220502215251993.png" alt="image-20220502215251993" style="zoom:25%;" />

- При **Hold** на **EN**-a, то *винаги* отиваме в това състояние:

$$
O_3 = 1 \\
O_4 = 0
$$

**Flow Chart:**

<img src="C:\Users\Gosho\Desktop\GitHub\8-bit-Computer\Pictures\image-20220508161237700.png" alt="image-20220508161237700"  />