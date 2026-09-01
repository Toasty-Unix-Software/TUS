	hint_nop21 word [0xe7a]
	hint_nop21 bx
	hint_nop21 dword [0xf8c]
	hint_nop21 dword [0xb40]
	hint_nop21 rax
	hint_nop21 rbp

%ifdef ERROR
	hint_nop21 r13w
	hint_nop21 r11d
	hint_nop21 r13
	hint_nop21 r17w
	hint_nop21 r21d
	hint_nop21 r17
%endif
