	hint_nop52 bx
	hint_nop52 ax
	hint_nop52 ebx
	hint_nop52 eax
	hint_nop52 qword [0x734]
	hint_nop52 rsi

%ifdef ERROR
	hint_nop52 r8w
	hint_nop52 r8d
	hint_nop52 r10
	hint_nop52 r23w
	hint_nop52 r25d
	hint_nop52 r21
%endif
