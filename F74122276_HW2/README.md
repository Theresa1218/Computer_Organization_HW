# Q1

## Log2(N)

以下是我計算 `log2(N)` 的思考過程：

| N  | 二進位 | log₂(N) 回傳值 |
|---:|:------:|---------------:|
| 1  | 1      | 0 |
| 2  | 10     | 1 |
| 4  | 100    | 2 |
| 8  | 1000   | 3 |
| 16 | 10000  | 4 |

因此最後要將 `log 值` 減一。

---

## 問題描述：在 Branch 前面累加指令，可能因跳轉造成 Count 錯誤

### Example 1

```asm
# others_cnt += 2
addi %[others_cnt], %[others_cnt], 2
beqz %[N], end_loop
srli %[N], %[N], 1
```

### Example 2

```asm
# others_cnt += 5
addi %[others_cnt], %[others_cnt], 5
beqz t1, end_reverse
slli t0, t0, 1
andi t2, %[b], 1
or t0, t0, t2
srli %[b], %[b], 1
```

由於 Branch 指令可能直接跳離目前的執行流程，如果在 Branch 前就先累加 `others_cnt`，可能會把實際上沒有執行到的指令也算進去，造成 Count 錯誤。

---

## 問題描述：PI 計算 — Float 運算與 `(-1)^i` 計算

### Float 不能直接拿整數來算

一開始直接拿整數去做 `fdiv.s`、`fadd.s` 這類浮點數運算，結果不符合預期。

原因如下：

- Float 指令不能直接使用一般整數值進行浮點運算。
- 必須先將整數轉換成 Float。
- 可以使用 `fcvt.s.w` 指令，將整數轉換成 Float，並存入 Floating-point Register。

例如：

```asm
fcvt.s.w f1, t2
fcvt.s.w f2, t3
```

### Float 的次方運算

需要計算：

```text
(-1)^i
```

但 Float 沒有直接進行次方運算的指令。

觀察數值變化：

```text
1 → -1 → 1 → -1 → ...
```

因此只需要每次使用 `0` 減去目前的值，即可在 `1` 與 `-1` 之間切換。

例如：

```text
0 - 1    = -1
0 - (-1) = 1
```

---

# Q2-1

## 問題描述：For 迴圈的判斷

因為 Spec 中沒有提供 `i` 的暫存器，所以我改用 `arr_size` 判斷迴圈是否結束。

因此每次迴圈應執行：

```text
arr_size--
```

而不是使用：

```text
i++
```

---

# Q2-2

## 問題描述：帶入 Example 理解 Vector 操作

以下使用此 Example：

```c
float h[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
float x[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
float y[10];
float id = 1.0;
int arr_size = 10;
```

### First Round

```text
vsetvli t0, 10, e32
```

得到：

```text
vl = 4
```

處理：

```text
index 0, 1, 2, 3
```

Vector 內容：

```text
v0 = {1, 2, 3, 4}
v1 = {10, 20, 30, 40}
```

計算：

```text
v2 = v0 * v1 + id
   = {11, 41, 91, 161}
```

寫回：

```text
y[0], y[1], y[2], y[3]
```

更新：

```text
arr_size - 4 = 6
array move = 4 * 4 bytes = 16 bytes
```

---

### Second Round

```text
vsetvli t0, 6, e32
```

得到：

```text
vl = 4
```

處理：

```text
index 4, 5, 6, 7
```

Vector 內容：

```text
v0 = {5, 6, 7, 8}
v1 = {50, 60, 70, 80}
```

計算：

```text
v2 = v0 * v1 + id
   = {251, 361, 491, 641}
```

寫回：

```text
y[4], y[5], y[6], y[7]
```

更新：

```text
arr_size - 4 = 2
array move = 4 * 4 bytes = 16 bytes
```

---

### Third Round

```text
vsetvli t0, 2, e32
```

得到：

```text
vl = 2
```

處理：

```text
index 8, 9
```

Vector 內容：

```text
v0 = {9, 10}
v1 = {90, 100}
```

計算：

```text
v2 = v0 * v1 + id
   = {811, 1001}
```

寫回：

```text
y[8], y[9]
```

更新：

```text
arr_size - 2 = 0
```

因此 `arr_size` 等於 0，跳出 Loop。

---

## 問題描述：調整 `arr_size` 的值

一開始把：

```asm
arr_size - t0
```

放在：

```asm
slli t0, t0, 2
```

後面，因此造成測資錯誤。

執行：

```asm
slli t0, t0, 2
```

之後，`t0` 的值已經從「Vector element 數量」變成「Byte offset」。

因此如果此時才使用：

```text
arr_size - t0
```

就會造成 `arr_size` 的值不正確。

所以應該先執行：

```text
arr_size - t0
```

再執行：

```asm
slli t0, t0, 2
```

避免影響 `arr_size` 的判斷。

---

# Q3-1 / Q3-2

## `[result] "+f"(single_floating_result)` / `[result] "+f"(double_floating_result)`

一開始使用 `flw` / `fsw` 從記憶體存取 `result`：

```asm
flw
fsw
```

這樣會造成額外的 Memory Access，也導致測資錯誤。

後來改成直接使用 Floating-point Register，例如使用 `f3` 暫存結果：

```asm
fmv.s f3, %[result]
fmv.s %[result], f3
```

Inline Assembly 中：

```c
[result] "+f"(single_floating_result)
```

代表 `single_floating_result` 會由 Compiler 自動配置到 Floating-point Register，因此不需要額外使用 `flw` / `fsw` 從 Memory 讀取或寫回。

`Q3-2` 的 `double_floating_result` 也使用相同概念處理。
