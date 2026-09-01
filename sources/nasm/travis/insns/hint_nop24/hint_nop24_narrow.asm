	hint_nop24 bp
	hint_nop24 si
	hint_nop24 edx
	hint_nop24 dword [0x917]
	hint_nop24 qword [0x849]
	hint_nop24 qword [0xe62]

%ifdef ERROR
	hint_nop24 r9w
	hint_nop24 r15d
	hint_nop24 r11
	hint_nop24 r23w
	hint_nop24 r30d
	hint_nop24 r26
%endif
