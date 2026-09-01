	hint_nop2 si
	hint_nop2 ax
	hint_nop2 edx
	hint_nop2 ebx
	hint_nop2 rcx
	hint_nop2 rcx

%ifdef ERROR
	hint_nop2 r11w
	hint_nop2 r13d
	hint_nop2 r13
	hint_nop2 r26w
	hint_nop2 r27d
	hint_nop2 r23
%endif
