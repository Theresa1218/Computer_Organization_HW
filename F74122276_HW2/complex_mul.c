// fmul_cnt += 4
"addi %[fmul_cnt], %[fmul_cnt], 4 \n\t"  
"fmul.s f1, %[A_Re], %[B_Re] \n\t"       // f1 = a.Re * b.Re
"fmul.s f2, %[A_Im], %[B_Im] \n\t"       // f2 = a.Im * b.Im
"fmul.s f3, %[A_Re], %[B_Im] \n\t"       // f2 = a.Re * b.Im
"fmul.s f4, %[A_Im], %[B_Re] \n\t"       // f2 = a.Im * b.Re 
// fsub_cnt += 1
"addi %[fsub_cnt], %[fsub_cnt], 1 \n\t"  
"fsub.s %[C_Re], f1, f2 \n\t"            // f3 = f1 - f2 (實部)
// fadd_cnt += 1
"addi %[fadd_cnt], %[fadd_cnt], 1 \n\t"  
"fadd.s %[C_Im], f3, f4 \n\t"            // f4 = f3 + f4 (虛部)