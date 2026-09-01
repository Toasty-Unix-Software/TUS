	hint_nop41 si
	hint_nop41 di
	hint_nop41 ebp
	hint_nop41 esi
	hint_nop41 rbx
	hint_nop41 rdx

%ifdef ERROR
	hint_nop41 r15w
	hint_nop41 r8d
	hint_nop41 r12
	hint_nop41 r21w
	hint_nop41 r26d
	hint_nop41 r28
%endif
