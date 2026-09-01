	hint_nop43 si
	hint_nop43 word [0xa0d]
	hint_nop43 dword [0x6c3]
	hint_nop43 eax
	hint_nop43 qword [0xccf]
	hint_nop43 qword [0x8e9]

%ifdef ERROR
	hint_nop43 r9w
	hint_nop43 r13d
	hint_nop43 r8
	hint_nop43 r22w
	hint_nop43 r23d
	hint_nop43 r23
%endif
