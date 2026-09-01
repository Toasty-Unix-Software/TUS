	hint_nop19 cx
	hint_nop19 word [0x2da]
	hint_nop19 ecx
	hint_nop19 dword [0xcff]
	hint_nop19 rbp
	hint_nop19 rdi

%ifdef ERROR
	hint_nop19 r13w
	hint_nop19 r8d
	hint_nop19 r12
	hint_nop19 r23w
	hint_nop19 r23d
	hint_nop19 r29
%endif
