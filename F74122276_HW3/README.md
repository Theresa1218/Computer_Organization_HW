# Q1 queue（FIFO）
1. queue: front -> `[0, 1, 2, 3]` <- back  
2. `front()` 回傳 `0` 代表最舊的元素 -> 準備被替換  
3. `pop()` 刪除 queue 最早進來的 `0` -> queue 變成 `[1, 2, 3]`  
4. `push(0)` 將 `0` 加入 queue 尾端 -> queue 變成 `[1, 2, 3, 0]`
=> 符合 FIFO（First In First Out）

# Q2-1 Matrix Transpose

## Optimized with Blocking
透過 **Blocking** => 在 Data 被替換前，充分 Access，優化 cache 存取行為，以提升 performance

## Example (方便理解)
1. Matrix Size: 設為 `4 × 4`
2. Data Type: 整數（`int`），介於 `0 ~ 1023`
3. Define Block: `BLOCK_SIZE = 2` 進行區塊處理
4. 原始矩陣:
src =
1   2   3   4
5   6   7   8
9  10  11  12
13 14  15  16
5. 分成 Block
Block (0,0)    Block (0,1)
1   2          3   4
5   6          7   8

Block (1,0)    Block (1,1)
9  10         11  12
13 14         15  16

### BLOCK improve
原始寫法會 `跨行讀 跨列寫`，cache miss 很高
=> 改為一個一個 `block`，存取都集中在相鄰記憶體
=> 大幅降低 cache miss，提高效能

### 效能比較（BLOCK_SIZE）
| BLOCK_SIZE | Improved Ratio            |      
|------------|---------------------------|
| 64         | 0.9897490742070868        | 
| 32         | 1.189963028466643         |
| 16         | 1.711600753414266         | 
| 8          | 1.7381469229345363 (Best) | 
| 4          | 1.5477344624131095        | 
`經過實測，BLOCK_SIZE = 8 是最佳選擇，超過此大小效能反而下降`

# Q2-2 Matrix Multiplication

##  Optimized with Blocking
透過 **Blocking** => 在 Data 被替換前，充分 Access，優化 cache 存取行為，以提升 performance

## Matrix Define
1. 矩陣 A: `i × k`  
2. 矩陣 B: `k × j`  
3. Output矩陣: `i × j`  
4. 運算公式：
   `Output[x][y] = A[x][0]*B[0][y] + A[x][1]*B[1][y] + ... + A[x][k-1]*B[k-1][y]`
5. 變數 ii、jj、kk 是 block 的起點，x、y、z 是在 block 內部處理用的變數

### BLOCK improve
將矩陣切成 block，每次處理一個區塊：
=> 每個 block 大小為 `BLOCK_SIZE × BLOCK_SIZE`
=> 將大矩陣拆成許多 block 進行乘法，再將 block 結果累加
=> 每次操作一小塊，減少記憶體區塊之間的切換，提升 cache hit rate
=> 避免反覆讀寫不連續記憶體，達到 `空間局部性` 最佳化

### 效能比較（BLOCK_SIZE）
| BLOCK_SIZE | Improved Ratio           |      
|------------|--------------------------|
| 16         | 3.1442681580649934       | 
| 8          | 4.291901354804909 (Best) | 
| 4          | 3.679930983953239        | 
`經過實測，BLOCK_SIZE = 8 是最佳選擇，超過此大小效能反而下降`