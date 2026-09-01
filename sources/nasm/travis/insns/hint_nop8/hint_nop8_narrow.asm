	hint_nop8 si
	hint_nop8 word [0xf58]
	hint_nop8 eax
	hint_nop8 edi
	hint_nop8 rax
	hint_nop8 qword [0x364]

%ifdef ERROR
	hint_nop8 r14w
	hint_nop8 r8d
	hint_nop8 r10
	hint_nop8 r31w
	hint_nop8 r30d
	hint_nop8 r26
%endif
