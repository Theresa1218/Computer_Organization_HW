# F74122276 HW1 檔案說明

## 1. `array_sort.c`
### C code 
    ```c
    for (int i = 0; i < arr_size - 1; i++) {
        for (int j = 0; j < arr_size - i -1; j++) {
            if (*(p_a + j) > *(p_a + j + 1)) {
                int tmp = *(p_a + j);
                *(p_a + j) = *(p_a + j + 1);
                *(p_a + j + 1) = tmp;
            }
        }
    }
    
### Explanation
根據`Bubble Sort if statement 中`比較 (p_a+j) 和 (p_a+j+1)存放的值，由於 Array 一個 word 佔 4 Bytes，所以要計算 j * 4 來獲取正確的記憶體位址。
因為對乘法的指令還不熟悉，所以參考了`講義 Chapter2 Page32`，使用`slli 指令`，將 j 左移 2 bits 相當於 *4，接著把 base address + offset 後，就可以把要比較的值從 memory 取出。
數值比較部分我使用`blt指令: if (arr[j+1] < arr[j])`，和 code 有稍微不同，假設條件成立就會執行 Swap 的`sw指令`，修改記憶體中的數值，否則 No_Swap，進行下一次 loop。

## 2. `array_search.c`
### C code 
    ```c
    for (int i = 0; i < arr_size; i++) {
        if (p_a[a] == target) {
            result = target;
            break;
        }
    }
    
### Explanation
一開始寫`addi t0, x0, 0 \n\t`宣告 i 變數，接著執行`loop 中的指令`，一樣使用第一題的技巧，offset 以左移來代表 *4，接著把 base address + offset 後，把要比較的值從 memory 取出，和 target 暫存器中存放的值比較看是否相同，用到`beq指令`。
假設條件成立就直接`goto found`，因為已經找到要回傳 index，在這裡我查資料，學到可以使用`mv指令`，把一個 register 的值複製到另一個 register，所以本來放 i 變數的 t0 register，就可以直接複製給 result。
若是條件不成立，則將 i+1，然後使用`bge指令(>=)`，確認是否已將 Array 掃過一遍，若成立就`goto done`，否則`goto loop`。

## 3. `linked_list_sort.c`
### C code
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

        if(!result) {
            result = temp;
            tail = temp;
        } else {
            tail->next = temp;
            tail = temp;
        }
    }
    if (a) tail->next = a; 
    if (b) tail->next = b; 
    
#### Block_C
    ```c
    cur = cur->next; 
    

### Explanation
一開始對 `linked_list` 的 `Node` 架構不熟悉，因此在將 data 從 memory 載入 register，或是將 data 從 register 寫回 memory 時，我一律使用 `lw/sw`。後來查資料後發現，`Node` 的結構包含一個 `int` 與一個 pointer，因此在 64-bit RISC-V 中，pointer 相關資料應使用 `ld/sd` 進行存取，才能正確處理 64-bit data。

在 `Block_C` 中，我也犯了一個錯誤：
```asm
sd %[cur], 8(%[cur])

但是程式是要對 memory 的 data 進行讀取，應改為 `ld` 非 `sd`。

在Block_A 中，遇到的困難是 C code 並沒有宣告 `slow` 和 `fast`，所以我不知道如何在 asm 中使用，像是一開始不確定 register 能當 base-adddress，所以寫
```asm
ld t2, 8(%[fast])

但是題目規定不能改 C code，後來發現可以用 register 代表它們，所以更改為```asm
ld t2, 8(t1)

其中 `t1` 代表 `fast`，這裡在處理 memory 和 register 之間資料，也必須使用`ld/sd`。
`Block_B` 中的 `temp` 也沒有在 C code 中宣告，而這個問題我已經在 `Block_A` 中了解清楚，所以使用 `t2` register 代表 `temp`。

然而遇到的瓶頸則是 Timing 問題，bug 原來是出現在 `tail_b` 中，因為它是從`a == NULL`跳過來的，我並沒有檢查 `b` 是否也等於 NULL，就直接寫

```c
tail->next = b;

所以答案錯誤，要補上

```asm
beq %[b], x0, finish

這裡的`tail_a`就不需要，因為它是從`b == NULL`跳過來，這行指令的上方已經檢查，確認過 a 不會是 NULL。
最後，以上的 Code 部分參考了 [YouTube](https://www.youtube.com/watch?v=2uDsyVmC96o)。
