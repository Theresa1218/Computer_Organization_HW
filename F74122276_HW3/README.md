# Q1 Queue（FIFO）

1. Queue：front → `[0, 1, 2, 3]` ← back
2. `front()` 回傳 `0`，代表最舊的元素，準備被替換
3. `pop()` 刪除 Queue 中最早進來的 `0`  
   → Queue 變成 `[1, 2, 3]`
4. `push(0)` 將 `0` 加入 Queue 尾端  
   → Queue 變成 `[1, 2, 3, 0]`

因此符合 FIFO（First In First Out）的特性。

---

# Q2-1 Matrix Transpose

## Optimized with Blocking

透過 **Blocking**，讓 Data 在被替換前能被充分 Access，改善 Cache 存取行為，以提升 Performance。

## Example

為方便理解，假設：

1. Matrix Size：`4 × 4`
2. Data Type：整數 `int`，數值介於 `0 ~ 1023`
3. Define Block：`BLOCK_SIZE = 2`
4. 原始矩陣：

```text
src =

1   2   3   4
5   6   7   8
9  10  11  12
13 14  15  16
```

5. 將 Matrix 分成 Blocks：

```text
Block (0,0)      Block (0,1)

1   2            3   4
5   6            7   8


Block (1,0)      Block (1,1)

9  10            11  12
13 14            15  16
```

## Blocking Improvement

原始寫法會產生：

```text
跨行讀、跨列寫
```

因此 Cache Miss 較高。

改成一個一個 `Block` 處理後：

- 存取會集中在相鄰的記憶體區域
- 降低 Cache Miss
- 提升 Cache Locality
- 改善整體效能

## 效能比較（BLOCK_SIZE）

| BLOCK_SIZE | Improved Ratio |
|-----------:|---------------:|
| 64 | 0.9897490742070868 |
| 32 | 1.189963028466643 |
| 16 | 1.711600753414266 |
| 8  | 1.7381469229345363 **(Best)** |
| 4  | 1.5477344624131095 |

經過實測，`BLOCK_SIZE = 8` 為最佳選擇；超過此大小後，效能反而下降。

---

# Q2-2 Matrix Multiplication

## Optimized with Blocking

透過 **Blocking**，讓 Data 在被替換前能被充分 Access，改善 Cache 存取行為，以提升 Performance。

## Matrix Definition

1. 矩陣 A：`i × k`
2. 矩陣 B：`k × j`
3. Output Matrix：`i × j`
4. 運算公式：

```text
Output[x][y]
= A[x][0] * B[0][y]
+ A[x][1] * B[1][y]
+ ...
+ A[x][k-1] * B[k-1][y]
```

5. `ii`、`jj`、`kk` 為各 Block 的起始位置；`x`、`y`、`z` 則是在 Block 內部處理資料時使用的變數。

## Blocking Improvement

將矩陣切成 Blocks，每次處理一個區塊：

- 每個 Block 大小為 `BLOCK_SIZE × BLOCK_SIZE`
- 將大矩陣拆成多個 Blocks 進行乘法
- 再將各 Block 的結果累加
- 每次只操作一小塊資料，減少不同記憶體區域之間的切換
- 提高 Cache Hit Rate
- 避免反覆存取不連續的記憶體，提升**空間局部性（Spatial Locality）**

## 效能比較（BLOCK_SIZE）

| BLOCK_SIZE | Improved Ratio |
|-----------:|---------------:|
| 16 | 3.1442681580649934 |
| 8  | 4.291901354804909 **(Best)** |
| 4  | 3.679930983953239 |

經過實測，`BLOCK_SIZE = 8` 為最佳選擇；超過此大小後，效能反而下降。
