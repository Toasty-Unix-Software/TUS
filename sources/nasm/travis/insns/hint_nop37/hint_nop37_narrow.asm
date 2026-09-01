	hint_nop37 ax
	hint_nop37 si
	hint_nop37 dword [0xeee]
	hint_nop37 dword [0x13e]
	hint_nop37 qword [0x615]
	hint_nop37 rcx

%ifdef ERROR
	hint_nop37 r11w
	hint_nop37 r14d
	hint_nop37 r13
	hint_nop37 r24w
	hint_nop37 r17d
	hint_nop37 r29
%endif
