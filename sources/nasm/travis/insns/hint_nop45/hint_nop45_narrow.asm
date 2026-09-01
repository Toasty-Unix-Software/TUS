	hint_nop45 cx
	hint_nop45 cx
	hint_nop45 dword [0xe9f]
	hint_nop45 ecx
	hint_nop45 rdi
	hint_nop45 rbx

%ifdef ERROR
	hint_nop45 r12w
	hint_nop45 r12d
	hint_nop45 r13
	hint_nop45 r21w
	hint_nop45 r23d
	hint_nop45 r21
%endif
