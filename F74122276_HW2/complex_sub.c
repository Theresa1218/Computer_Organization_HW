// fsub_cnt += 2
"addi %[fsub_cnt], %[fsub_cnt], 2 \n\t" 
"fsub.s %[C_Re], %[A_Re], %[B_Re] \n\t"  // result.Re = a.Re - b.Re
"fsub.s %[C_Im], %[A_Im], %[B_Im] \n\t"  // result.Im = a.Im - b.Im