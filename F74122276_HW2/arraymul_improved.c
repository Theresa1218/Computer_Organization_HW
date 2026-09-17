"loop: \n\t"
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "vsetvli t0, %[arr_size], e32 \n\t"           // t0 = vector length (each vector has 32 bits float)
    // lw_cnt += 2
    "addi %[lw_cnt], %[lw_cnt], 2 \n\t"
    "vle32.v v0, 0(%[h]) \n\t"                    // v0 = h
    "vle32.v v1, 0(%[x]) \n\t"                    // v1 = x
    // fmul_cnt += 1
    "addi %[fmul_cnt], %[fmul_cnt], 1 \n\t" 
    "vfmul.vv v2, v0, v1 \n\t"                    // v2 = v0 * v1      
    // fadd_cnt += 1
    "addi %[fadd_cnt], %[fadd_cnt], 1 \n\t"  
    "vfadd.vf v2, v2, %[id] \n\t"                 // v2 = v2 + id
    // sw_cnt += 1
    "addi %[sw_cnt], %[sw_cnt], 1 \n\t"
    "vse32.v v2, 0(%[y]) \n\t"                    // y = v2 
    // sub_cnt += 1
    "addi %[sub_cnt], %[sub_cnt], 1 \n\t"
    "sub %[arr_size], %[arr_size], t0 \n\t"       // arr_size - vector length
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "slli t0, t0, 2 \n\t"                         // single float = 4 bytes
    // add_cnt += 3
    "addi %[add_cnt], %[add_cnt], 3 \n\t"
    "add %[h], %[h], t0 \n\t"
    "add %[x], %[x], t0 \n\t"
    "add %[y], %[y], t0 \n\t"
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "bne %[arr_size], zero, loop \n\t"           // if (arr_size != 0) goto loop