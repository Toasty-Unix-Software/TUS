	hint_nop30 di
	hint_nop30 word [0xd98]
	hint_nop30 dword [0x519]
	hint_nop30 esi
	hint_nop30 rax
	hint_nop30 qword [0x922]

%ifdef ERROR
	hint_nop30 r11w
	hint_nop30 r12d
	hint_nop30 r10
	hint_nop30 r30w
	hint_nop30 r31d
	hint_nop30 r29
%endif
