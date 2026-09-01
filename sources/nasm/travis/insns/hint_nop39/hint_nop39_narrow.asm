	hint_nop39 di
	hint_nop39 word [0x9d0]
	hint_nop39 esi
	hint_nop39 ebp
	hint_nop39 qword [0x8a1]
	hint_nop39 rbp

%ifdef ERROR
	hint_nop39 r12w
	hint_nop39 r12d
	hint_nop39 r13
	hint_nop39 r23w
	hint_nop39 r23d
	hint_nop39 r23
%endif
