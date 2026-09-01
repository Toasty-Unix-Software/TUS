	hint_nop15 word [0xde9]
	hint_nop15 bx
	hint_nop15 dword [0xece]
	hint_nop15 ebx
	hint_nop15 rcx
	hint_nop15 rdx

%ifdef ERROR
	hint_nop15 r12w
	hint_nop15 r15d
	hint_nop15 r12
	hint_nop15 r20w
	hint_nop15 r19d
	hint_nop15 r25
%endif
