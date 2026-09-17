// fadd_cnt += 2
"addi %[fadd_cnt], %[fadd_cnt], 2 \n\t"  
"fadd.s %[C_Re], %[A_Re], %[B_Re] \n\t"  // result.Re = a.Re + b.Re
"fadd.s %[C_Im], %[A_Im], %[B_Im] \n\t"  // result.Im = a.Im + b.Im