func m_abs
Ppt %ax
ALUCall %ax %fl .xor
Gnum %bx 0x01
ALUCall %ax %zr .inc
Ps %ax
funcend

func pow
Ppt %ax
Ppt %bx
Jeq %ax %zr ...
Jeq %bx %zr ...
ALUCall %ax %ax .mul
ALUCall %bx %zr .dec
Jne %bx %zr 0xf5
funcend