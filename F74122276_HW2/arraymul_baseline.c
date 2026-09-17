"loop: \n\t"
    // flw_cnt += 2
    "addi %[flw_cnt], %[flw_cnt], 2 \n\t" 
    "flw f0, 0(%[h]) \n\t"        // f0 = h
    "flw f1, 0(%[x]) \n\t"        // f1 = x
    // fmul_cnt += 1
    "addi %[fmul_cnt], %[fmul_cnt], 1 \n\t"
    "fmul.s f0, f0, f1 \n\t"      // f0 = h * x
    // fadd_cnt += 1
    "addi %[fadd_cnt], %[fadd_cnt], 1 \n\t"
    "fadd.s f0, f0, %[id] \n\t"   // f0 = h * x + id
    // fsw_cnt += 1
    "addi %[fsw_cnt], %[fsw_cnt], 1 \n\t"
    "fsw f0, 0(%[y]) \n\t"        // y = f0
    // add_cnt += 4
    "addi %[add_cnt], %[add_cnt], 4 \n\t"
    "addi %[h], %[h], 4 \n\t"     // p_h++
    "addi %[x], %[x], 4 \n\t"     // p_x++
    "addi %[y], %[y], 4 \n\t"     // p_y++
    "addi %[arr_size], %[arr_size], -1 \n\t"    // arr_size--
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t" 
    "bne %[arr_size], zero, loop \n\t"          // if (arr_size != 0) goto loop