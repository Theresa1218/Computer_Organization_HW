// add_cnt += 1
"addi %[add_cnt], %[add_cnt], 1 \n\t"
"addi t0, zero, 0 \n\t" // t0 = 0 
"loop: \n\t"
    // dlw_cnt += 2
    "addi %[dlw_cnt], %[dlw_cnt], 2 \n\t" 
    "fld f0, 0(%[h]) \n\t"           // f0 = h
    "fld f1, 0(%[x]) \n\t"           // f1 = x
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "fmv.d f3, %[result] \n\t"       // f3 = result
    // dmul_cnt += 2
    "addi %[dmul_cnt], %[dmul_cnt], 2 \n\t"
    "fmul.d f2, f0, f1 \n\t"         // f2 = h * x
    "fmul.d f3, f2, %[result] \n\t"  // f3 = f3 * h * x
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "fmv.d %[result], f3 \n\t"       // result = f3
    // add_cnt += 3
    "addi %[add_cnt], %[add_cnt], 3 \n\t" 
    "addi %[h], %[h], 8 \n\t"        // p_h++
    "addi %[x], %[x], 8 \n\t"        // p_x++
    "addi t0, t0, 1 \n\t"            // i++
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "blt t0, %[arr_size], loop \n\t" // if (i < arr_size) goto loop