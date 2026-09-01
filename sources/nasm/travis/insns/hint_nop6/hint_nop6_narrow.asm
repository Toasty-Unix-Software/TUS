	hint_nop6 cx
	hint_nop6 bx
	hint_nop6 esi
	hint_nop6 eax
	hint_nop6 rdi
	hint_nop6 qword [0xed4]

%ifdef ERROR
	hint_nop6 r14w
	hint_nop6 r8d
	hint_nop6 r15
	hint_nop6 r23w
	hint_nop6 r22d
	hint_nop6 r19
%endif
