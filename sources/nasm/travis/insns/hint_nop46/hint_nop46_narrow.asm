	hint_nop46 ax
	hint_nop46 cx
	hint_nop46 dword [0x4c3]
	hint_nop46 ecx
	hint_nop46 rbp
	hint_nop46 rdx

%ifdef ERROR
	hint_nop46 r10w
	hint_nop46 r14d
	hint_nop46 r12
	hint_nop46 r30w
	hint_nop46 r30d
	hint_nop46 r20
%endif
