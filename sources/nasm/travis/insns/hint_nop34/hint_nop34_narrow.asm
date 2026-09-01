	hint_nop34 ax
	hint_nop34 si
	hint_nop34 esi
	hint_nop34 ebx
	hint_nop34 qword [0x3b4]
	hint_nop34 rcx

%ifdef ERROR
	hint_nop34 r12w
	hint_nop34 r11d
	hint_nop34 r9
	hint_nop34 r24w
	hint_nop34 r16d
	hint_nop34 r24
%endif
