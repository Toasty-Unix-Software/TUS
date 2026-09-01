	hint_nop58 di
	hint_nop58 di
	hint_nop58 dword [0xa69]
	hint_nop58 dword [0xdce]
	hint_nop58 rcx
	hint_nop58 rbx

%ifdef ERROR
	hint_nop58 r10w
	hint_nop58 r9d
	hint_nop58 r12
	hint_nop58 r24w
	hint_nop58 r27d
	hint_nop58 r18
%endif
