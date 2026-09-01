	hint_nop60 word [0x6e1]
	hint_nop60 word [0xed4]
	hint_nop60 edi
	hint_nop60 dword [0x499]
	hint_nop60 rdx
	hint_nop60 rdi

%ifdef ERROR
	hint_nop60 r15w
	hint_nop60 r10d
	hint_nop60 r9
	hint_nop60 r16w
	hint_nop60 r29d
	hint_nop60 r17
%endif
