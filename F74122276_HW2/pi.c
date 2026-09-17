// add_cnt += 2
"addi %[add_cnt], %[add_cnt], 2 \n\t"
"addi t1, zero, 0 \n\t" // t1 = i = 0
"addi t2, zero, 1 \n\t" // t2 = (-1)^i

"pi_loop: \n\t"
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "slli t3, t1, 1 \n\t"          // t3 = 2i
    // add_cnt += 1
    "addi %[add_cnt], %[add_cnt], 1 \n\t"
    "addi t3, t3, 1 \n\t"          // t3 = 2i + 1
    // others_cnt += 2
    "addi %[others_cnt], %[others_cnt], 2 \n\t"
    "fcvt.s.w f1, t2 \n\t"         // f1 = (-1)^i
    "fcvt.s.w f2, t3 \n\t"         // f2 = t3 = 2i + 1
    // fdiv_cnt += 1
    "addi %[fdiv_cnt], %[fdiv_cnt], 1 \n\t"
    "fdiv.s f1, f1, f2 \n\t"       // f1 = term = (-1)^i / (2i + 1)
    // fadd_cnt += 1
    "addi %[fadd_cnt], %[fadd_cnt], 1 \n\t"
    "fadd.s %[pi], %[pi], f1 \n\t" // pi += term 
    // add_cnt += 1
    "addi %[add_cnt], %[add_cnt], 1 \n\t"
    "addi t1, t1, 1 \n\t"          // i = i + 1
    // sub_cnt += 1
    "addi %[sub_cnt], %[sub_cnt], 1 \n\t"
    "sub t2, zero, t2 \n\t"        // t2 = 0 - t2 = (-1)^i 
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "blt t1, %[N], pi_loop \n\t"