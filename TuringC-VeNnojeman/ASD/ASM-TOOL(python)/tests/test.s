include <std_math.s>
Gnum %ax 0x01
Gnum %bx 0x02
Ps %bx
Ps %ax
callfunc std_math.m_abs
Out
Ret
