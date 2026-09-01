	hint_nop7 word [0x4cc]
	hint_nop7 bp
	hint_nop7 ebp
	hint_nop7 dword [0x838]
	hint_nop7 rdx
	hint_nop7 rsi

%ifdef ERROR
	hint_nop7 r11w
	hint_nop7 r9d
	hint_nop7 r12
	hint_nop7 r29w
	hint_nop7 r26d
	hint_nop7 r27
%endif
