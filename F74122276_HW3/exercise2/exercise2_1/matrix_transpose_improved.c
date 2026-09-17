#define BLOCK_SIZE 8

void matrix_transpose(int n, int *dst, int *src) {
    // Implement your code here

    // 橫向分區塊(32 rows)
    for (int i = 0 ; i < n ; i += BLOCK_SIZE) {
        // 縱向分區塊(32 cols)
        for (int j = 0 ; j < n ; j += BLOCK_SIZE) {
            // 在一個 block 中進行轉置(row)
            // ii -> block row index
            for (int ii = i ; ii < i + BLOCK_SIZE && ii < n ; ii++) {
                    // 在一個 block 中進行轉置(col)
                    // jj -> block column index
                    for (int jj = j ; jj < j + BLOCK_SIZE && jj < n ; jj++) {
                        // 將 src[ii][jj] 轉置至 dst[jj][ii]
                        dst[jj * n + ii] = src[ii * n + jj];
                    }
            }
        }
    }
}