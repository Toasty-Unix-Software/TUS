	hint_nop27 si
	hint_nop27 di
	hint_nop27 dword [0xd9a]
	hint_nop27 dword [0xda0]
	hint_nop27 qword [0xddf]
	hint_nop27 qword [0x74e]

%ifdef ERROR
	hint_nop27 r15w
	hint_nop27 r14d
	hint_nop27 r8
	hint_nop27 r20w
	hint_nop27 r23d
	hint_nop27 r31
%endif
