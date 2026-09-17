#define BLOCK_SIZE 8

void matrix_multiply(int *a, int *b, int *output, int i,
                           int k, int j) {
    // Implement your code here
    // 對矩陣 A 的 row 進行分區 (外層 block)
    for (int ii = 0 ; ii < i ; ii += BLOCK_SIZE) {
        // 對矩陣 B 的 column 進行分區 (外層 block)
        for (int jj = 0 ; jj < j ; jj += BLOCK_SIZE) {
            for (int kk = 0 ; kk < k ; kk += BLOCK_SIZE) {
                // x -> 在一個 block 中 row index
                for (int x = ii ; x < ii + BLOCK_SIZE && x < i ; x++) {
                    // y -> 在一個 block 中 column index
                    for (int y = jj ; y < jj + BLOCK_SIZE && y < j ; y++) {
                        int sum = 0;  // 初始化結果
                        for (int z = kk ; z < kk + BLOCK_SIZE && z < k ; z++) {
                            sum += a[x * k + z] * b[z * j + y]; // 累積 A[x][z] * B[z][y] 的結果
                        }
                        // 將 block 的乘積累加至 output[x][y]
                        output[x * j + y] += sum;
                    }
                }
            }
        }
    }
}