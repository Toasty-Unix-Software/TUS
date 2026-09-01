	hint_nop3 cx
	hint_nop3 word [0xcc5]
	hint_nop3 edx
	hint_nop3 dword [0x650]
	hint_nop3 qword [0xf38]
	hint_nop3 rsi

%ifdef ERROR
	hint_nop3 r10w
	hint_nop3 r10d
	hint_nop3 r10
	hint_nop3 r20w
	hint_nop3 r26d
	hint_nop3 r27
%endif
