#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }
    int arr_size;
    fscanf(input, "%d", &arr_size);
    int arr[arr_size];

    // Read integers from input file into the array
    for (int i = 0; i < arr_size; i++) {
        int data;
        fscanf(input, "%d", &data);
        arr[i] = data;
    }
    fclose(input);

    int *p_a = &arr[0];
    
    // array a bubble sort
    /* Original C code segment
    for (int i = 0; i < arr_size - 1; i++) {
        for (int j = 0; j < arr_size - i -1; j++) {
            if (*(p_a + j) > *(p_a + j + 1)) {
                int tmp = *(p_a + j);
                *(p_a + j) = *(p_a + j + 1);
                *(p_a + j + 1) = tmp;
            }
        }
    }
    */

    for (int i = 0; i < arr_size - 1; i++) {
        for (int j = 0; j < arr_size - i - 1; j++) {
            asm volatile(
                "slli t1, %[j], 2 \n\t"   // j * 4
                "add t2, %[p_a], t1 \n\t" // *(p_a + j) address
                "lw t3, 0(t2) \n\t"       // arr[j]
                "lw t4, 4(t2) \n\t"       // arr[j+1]
                "blt t4, t3, Swap \n\t"   // if (arr[j+1] < arr[j]) Swap
                "j No_Swap \n\t"          // else No_Swap

                "Swap: \n\t"
                    "sw t3, 4(t2) \n\t"       // arr[j+1] = arr[j]
                    "sw t4, 0(t2) \n\t"       // arr[j] = arr[j+1]
                                                                        
                "No_Swap: \n\t"

                :
                :[p_a] "r"(p_a), [j] "r"(j)
                :"t1", "t2", "t3", "t4"
            );
        }
    }
    p_a = &arr[0];
    for (int i = 0; i < arr_size; i++)
        printf("%d ", *p_a++);
    printf("\n");   
    return 0;
}     