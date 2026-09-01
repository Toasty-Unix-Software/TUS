	hint_nop48 word [0x5fc]
	hint_nop48 word [0xd9e]
	hint_nop48 edi
	hint_nop48 dword [0x994]
	hint_nop48 qword [0x2b7]
	hint_nop48 qword [0x72c]

%ifdef ERROR
	hint_nop48 r9w
	hint_nop48 r10d
	hint_nop48 r11
	hint_nop48 r21w
	hint_nop48 r25d
	hint_nop48 r22
%endif
