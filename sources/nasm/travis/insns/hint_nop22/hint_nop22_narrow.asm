	hint_nop22 word [0x702]
	hint_nop22 si
	hint_nop22 esi
	hint_nop22 dword [0xf0e]
	hint_nop22 qword [0xb87]
	hint_nop22 rbp

%ifdef ERROR
	hint_nop22 r8w
	hint_nop22 r13d
	hint_nop22 r8
	hint_nop22 r16w
	hint_nop22 r25d
	hint_nop22 r22
%endif
