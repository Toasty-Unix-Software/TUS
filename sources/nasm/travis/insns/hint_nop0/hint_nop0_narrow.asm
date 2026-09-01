	hint_nop0 di
	hint_nop0 dx
	hint_nop0 ebx
	hint_nop0 ecx
	hint_nop0 rbp
	hint_nop0 rbx

%ifdef ERROR
	hint_nop0 r14w
	hint_nop0 r10d
	hint_nop0 r10
	hint_nop0 r20w
	hint_nop0 r26d
	hint_nop0 r29
%endif
