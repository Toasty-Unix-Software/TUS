	hint_nop13 bx
	hint_nop13 di
	hint_nop13 dword [0x5e1]
	hint_nop13 edi
	hint_nop13 rcx
	hint_nop13 rax

%ifdef ERROR
	hint_nop13 r10w
	hint_nop13 r11d
	hint_nop13 r11
	hint_nop13 r16w
	hint_nop13 r25d
	hint_nop13 r18
%endif
