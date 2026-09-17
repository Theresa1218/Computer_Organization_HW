"loop: \n\t"
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "beqz %[N], end_loop \n\t"  
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "srli %[N], %[N], 1 \n\t"      // N >> 1
    // add_cnt += 1
    "addi %[add_cnt], %[add_cnt], 1 \n\t"
    "addi %[log], %[log], 1 \n\t" 
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "j loop \n\t"         
"end_loop: \n\t"
    // add_cnt += 1
    "addi %[add_cnt], %[add_cnt], 1 \n\t"
    "addi %[log], %[log], -1 \n\t" // log - 1