#include <stdio.h>

int arraySearch(int *p_a, int arr_size, int target)
{
    int result = -1;
    /*
    for (int i = 0; i < arr_size; i++) {
        if (p_a[a] == target) {
            result = target;
            break;
        }
    }
    */
    asm volatile(
        "addi t0, x0, 0 \n\t"                // t0 = i 
        "loop: \n\t"
            "slli t1, t0, 2 \n\t"            // offset = i * 4
            "add t1, %[p_a], t1 \n\t"        // t1 = p_a + offset
            "lw t2, 0(t1) \n\t"              // t2 = arr[i]
            "beq t2, %[target], found \n\t"  // if (t2 == target) goto found

            "addi t0, t0, 1 \n\t"            // i++
            "bge t0, %[arr_size], done \n\t" // i >= arr_size    
            "j loop \n\t"                    // next loop

        "found: \n\t"                    
            "mv %[result], t0 \n\t"          // result = i

        "done: \n\t"                     

        : [result] "+r"(result)
        : [p_a] "r"(p_a), [arr_size] "r"(arr_size), [target] "r"(target)
        : "t0", "t1", "t2"
    );

    return result;
}

// Main function to test the implementation
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
    int target;
    fscanf(input, "%d", &target);
    fclose(input);

    int *p_a = &arr[0];

    int index = arraySearch(p_a, arr_size, target);

    // Print the result
    printf("%d ", index);
    printf("\n");

    return 0;
}