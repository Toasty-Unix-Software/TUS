	hint_nop62 cx
	hint_nop62 si
	hint_nop62 ebx
	hint_nop62 dword [0xc25]
	hint_nop62 rbp
	hint_nop62 rdx

%ifdef ERROR
	hint_nop62 r15w
	hint_nop62 r11d
	hint_nop62 r13
	hint_nop62 r23w
	hint_nop62 r20d
	hint_nop62 r23
%endif
