# Q1 

## Log2(N)：以下是我計算 `log2(N)` 的思考過程

| N    |  二進位       |  log₂(N) 回傳值 |
|------|---------------|----------------|
| 1    |        1      | 0              |
| 2    |       10      | 1              |
| 4    |      100      | 2              |
| 8    |     1000      | 3              |
| 16   |    10000      | 4              |
=> 最後要把 `log值` 減一

## 問題描述：在 branch 前面累加指令 => 可能會跳走 => count 出錯

### 例如:
1.  // others_cnt += 2
    "addi %[others_cnt], %[others_cnt], 2 \n\t"
    "beqz %[N], end_loop \n\t"  
    "srli %[N], %[N], 1 \n\t" 
2.  // others_cnt += 5
    "addi %[others_cnt], %[others_cnt], 5 \n\t"
    "beqz t1, end_reverse \n\t" 
    "slli t0, t0, 1 \n\t"    
    "andi t2, %[b], 1 \n\t"     
    "or t0, t0, t2 \n\t"       
    "srli %[b], %[b], 1 \n\t"  

## 問題描述：PI計算 -> float 運算 / i^(-1) 計算

### float 不能直接拿整數來算
一開始直接拿整數去做 `fdiv.s`、`fadd.s` 這種浮點數除法、加法，結果非預期
=> float 指令不能直接使用整數
=> 要先把整數轉成 float 才能運算
=> `fcvt.s.w`指令，把整數轉成 float，然後放到 float 暫存器中
EX1: `fcvt.s.w f1, t2` / EX2: `fcvt.s.w f2, t3`

### float 的次方運算
次方運算: (-1)^i -> float 沒有次方的指令
=> 其實就是從 1 → -1 → 1 → -1 不斷跳動
=> 只要每次用 0 減去現在的值就可以實現
EX1: `0 - (1) = -1` ； EX2: `0 - (-1) = 1`

# Q2-1 

## 問題描述：for 迴圈的判斷
因為 Spec 中沒有 i 的暫存器，所以我使用 arr_size 來判斷，應為 arr_size--，而非 i++

# Q2-2

## 問題描述：帶入 example 理解
float h[10] = {1,2,3,4,5,6,7,8,9,10}
float x[10] = {10,20,30,40,50,60,70,80,90,100}
float y[10]
float id = 1.0
int arr_size = 10

### first round
vsetvli t0, 10, e32 -> vl = 4
`處理 index 0 1 2 3`
=> v0 = {1,2,3,4}
=> v1 = {10,20,30,40}
=> v2 = v0 * v1 + id = {11, 41, 91, 161}
=> sw: y[0] y[1] y[2] y[3] 
arr_size -4 -> 6
array move 4 * 4 bytes = 16 bytes

### second round
vsetvli t0, 6, e32 -> vl = 4
`處理 index 4 5 6 7`
=> v0 = {5,6,7,8}
=> v1 = {50,60,70,80}
=> v2 = v0 * v1 + id = {251, 361, 491, 641}
=> sw: y[4] y[5] y[6] y[7] 
arr_size -4 -> 2
array move 4 * 4 bytes = 16 bytes

### third round
vsetvli t0, 2, e32 -> vl = 2
`處理 index 8 9`
=> v0 = {9,10}
=> v1 = {90,100}
=> v2 = v0 * v1 + id = {811, 1001}
=> sw: y[8] y[9] 
arr_size -2 -> equal to zero 
array move 4 * 4 bytes = 16 bytes
=> 跳出 loop

## 問題描述：調整arr_size 的值
一開始把  `arr_size - t0` 的指令放在 `slli t0, t0, 2 \n\t` 指令的後面，所以測資是錯的
=> 這樣會導致 arr_size 的值不正確，因為 t0 的已經被更改
=> 所以要把 `arr_size - t0` 的指令放在 `slli t0, t0, 2 \n\t` 指令的前面，才不會影響到 arr_size 的判斷

# Q3-1 / Q3-2

## [result] "+f"(single_floating_result) / [result] "+f"(double_floating_result)

1. 一開始用 `flw`/`fsw` 從記憶體存取 `result`
=> 導致額外的記憶體操作與測資錯誤
2. 後來改用 `f3` 暫存器直接累計結果
=> 使用 `fmv.s f3, %[result]` / `fmv.s %[result], f3` 指令
3. `[result] "+f"(single_floating_result)` 
=> 代表 `single_floating_result` 會自動對應到某個 float 暫存器，所以不需要從記憶體存取
=> 同理 3-2 一樣的做法