	hint_nop20 si
	hint_nop20 si
	hint_nop20 ebp
	hint_nop20 edx
	hint_nop20 qword [0xf3e]
	hint_nop20 qword [0x881]

%ifdef ERROR
	hint_nop20 r8w
	hint_nop20 r9d
	hint_nop20 r10
	hint_nop20 r17w
	hint_nop20 r16d
	hint_nop20 r29
%endif
