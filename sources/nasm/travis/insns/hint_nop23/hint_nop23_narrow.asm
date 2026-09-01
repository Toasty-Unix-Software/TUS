	hint_nop23 bp
	hint_nop23 si
	hint_nop23 ebx
	hint_nop23 dword [0xaa7]
	hint_nop23 rdx
	hint_nop23 qword [0x949]

%ifdef ERROR
	hint_nop23 r15w
	hint_nop23 r9d
	hint_nop23 r11
	hint_nop23 r28w
	hint_nop23 r28d
	hint_nop23 r20
%endif
