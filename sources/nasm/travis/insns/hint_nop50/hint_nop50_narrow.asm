	hint_nop50 cx
	hint_nop50 di
	hint_nop50 edi
	hint_nop50 dword [0x10c]
	hint_nop50 rdi
	hint_nop50 rbx

%ifdef ERROR
	hint_nop50 r12w
	hint_nop50 r9d
	hint_nop50 r14
	hint_nop50 r17w
	hint_nop50 r19d
	hint_nop50 r23
%endif
