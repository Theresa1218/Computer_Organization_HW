// add_cnt += 1
"addi %[add_cnt], %[add_cnt], 1 \n\t"
"addi t0, zero, 0 \n\t" // t0 = 0 (reversed)
// others_cnt += 1
"addi %[others_cnt], %[others_cnt], 1 \n\t"
"mv t1, %[m] \n\t"      // t1 = m
"reverse_loop: \n\t"
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "beqz t1, end_reverse \n\t" // if (t1 == 0) goto end_reverse
    // others_cnt += 4
    "addi %[others_cnt], %[others_cnt], 4 \n\t"
    "slli t0, t0, 1 \n\t"       // reversed <<= 1 
    "andi t2, %[b], 1 \n\t"     // t2 = b & 1 (get LSB)
    "or t0, t0, t2 \n\t"        // reversed |= t2
    "srli %[b], %[b], 1 \n\t"   // b >>= 1（右移一位）
    // add_cnt += 1
    "addi %[add_cnt], %[add_cnt], 1 \n\t"
    "addi t1, t1, -1 \n\t"      // m--
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "j reverse_loop \n\t"
"end_reverse: \n\t"
    // others_cnt += 1
    "addi %[others_cnt], %[others_cnt], 1 \n\t"
    "mv %[b], t0 \n\t"        