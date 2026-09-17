# F74122276 HW1 檔案說明

## 1. `array_sort.c`

### C Code

```c
for (int i = 0; i < arr_size - 1; i++) {
    for (int j = 0; j < arr_size - i - 1; j++) {
        if (*(p_a + j) > *(p_a + j + 1)) {
            int tmp = *(p_a + j);
            *(p_a + j) = *(p_a + j + 1);
            *(p_a + j + 1) = tmp;
        }
    }
}
```

### Explanation

根據 Bubble Sort 中的 `if statement`，需要比較 `(p_a + j)` 和 `(p_a + j + 1)` 所存放的值。

由於 Array 中一個 word 佔 4 Bytes，所以需要計算 `j * 4` 來取得正確的記憶體位址。

因為一開始對乘法相關指令還不熟悉，所以參考了講義 Chapter 2 Page 32，使用 `slli` 指令，將 `j` 左移 2 bits，相當於乘以 4。接著將 base address 加上 offset，就可以從 memory 中取出要比較的值。

數值比較部分使用：

```asm
blt
```

也就是判斷：

```c
arr[j + 1] < arr[j]
```

和原本 C code 的寫法稍微不同。

若條件成立，就執行 Swap，使用 `sw` 指令修改 memory 中的數值；否則跳到 `No_Swap`，進行下一次 loop。

---

## 2. `array_search.c`

### C Code

```c
for (int i = 0; i < arr_size; i++) {
    if (p_a[i] == target) {
        result = i;
        break;
    }
}
```

### Explanation

一開始使用：

```asm
addi t0, x0, 0
```

宣告 `i` 變數，接著進入 loop。

和第一題相同，使用左移方式計算 offset：

```asm
slli
```

因為一個 array element 為 4 Bytes，所以將 index 左移 2 bits，等同於乘以 4。

之後將 base address 加上 offset，從 memory 取出對應位置的值，再與 `target` register 中存放的值進行比較。

比較時使用：

```asm
beq
```

若條件成立，就跳到：

```asm
found
```

因為此時已經找到要回傳的 index。

這裡我另外查資料學到可以使用 `mv` 指令，將一個 register 的值複製到另一個 register，因此原本存放 `i` 的 `t0` register 可以直接複製給 `result`。

若條件不成立，則將 `i + 1`，接著使用：

```asm
bge
```

判斷是否已經將整個 Array 掃描完成。

若成立則跳到：

```asm
done
```

否則繼續回到：

```asm
loop
```

---

## 3. `linked_list_sort.c`

### C Code

#### Block_A

```c
if (head == NULL || head->next == NULL) {
    *firstHalf = head;
    *secondHalf = NULL;
    return;
}

Node *slow = head;
Node *fast = head->next;

while (fast != NULL && fast->next != NULL) {
    slow = slow->next;
    fast = fast->next->next;
}

*firstHalf = head;
*secondHalf = slow->next;
slow->next = NULL;
```

#### Block_B

```c
if (a == NULL) return b;
if (b == NULL) return a;

while (a != NULL && b != NULL) {
    Node *temp = NULL;

    if (a->data <= b->data) {
        temp = a;
        a = a->next;
    } else {
        temp = b;
        b = b->next;
    }

    if (!result) {
        result = temp;
        tail = temp;
    } else {
        tail->next = temp;
        tail = temp;
    }
}

if (a) tail->next = a;
if (b) tail->next = b;
```

#### Block_C

```c
cur = cur->next;
```

### Explanation

一開始對 `linked_list` 的 `Node` 架構不熟悉，因此在將 data 從 memory 載入 register，或是將 data 從 register 寫回 memory 時，我一律使用 `lw/sw`。

後來查資料後發現，`Node` 的結構包含一個 `int` 與一個 pointer，因此在 64-bit RISC-V 中，pointer 相關資料應使用 `ld/sd` 進行存取，才能正確處理 64-bit data。

### Block_C

在 `Block_C` 中，我一開始寫成：

```asm
sd %[cur], 8(%[cur])
```

但是這段程式的目的是從 memory 讀取 data，因此應該使用 `ld`，而不是 `sd`：

```asm
ld %[cur], 8(%[cur])
```

### Block_A

在 `Block_A` 中，遇到的困難是 C code 並沒有宣告 `slow` 和 `fast`，因此一開始不知道該如何在 assembly 中使用。

原本曾寫成：

```asm
ld t2, 8(%[fast])
```

但題目規定不能修改 C code。

後來發現可以直接使用 register 代表這些 pointer，因此改成：

```asm
ld t2, 8(t1)
```

其中 `t1` 代表 `fast`。

這也讓我了解 register 可以直接作為 base address，而在 memory 和 register 之間處理 pointer data 時，也必須使用 `ld/sd`。

### Block_B

在 `Block_B` 中，`temp` 同樣沒有在 C code 中宣告。

因為在 `Block_A` 中已經理解這個問題，因此直接使用 `t2` register 代表 `temp`。

另外，在 `Block_B` 中遇到的主要瓶頸是 Timing 問題。

最後發現 bug 出現在 `tail_b`。

`tail_b` 是從：

```c
a == NULL
```

的情況跳轉過來，但我原本沒有先確認 `b` 是否也等於 `NULL`，就直接執行：

```c
tail->next = b;
```

因此會造成錯誤。

後來補上：

```asm
beq %[b], x0, finish
```

先確認 `b` 是否為 `NULL`。

相較之下，`tail_a` 不需要額外檢查 `a == NULL`，因為它是從：

```c
b == NULL
```

的情況跳轉過來，而在前面的指令中已經確認過 `a` 不為 `NULL`。

---

## Reference

以上 Code 部分參考：

[YouTube](https://www.youtube.com/watch?v=2uDsyVmC96o)
