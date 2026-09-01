	hint_nop1 cx
	hint_nop1 word [0x414]
	hint_nop1 ecx
	hint_nop1 edi
	hint_nop1 rcx
	hint_nop1 rdi

%ifdef ERROR
	hint_nop1 r9w
	hint_nop1 r12d
	hint_nop1 r15
	hint_nop1 r26w
	hint_nop1 r17d
	hint_nop1 r21
%endif
