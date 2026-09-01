	hint_nop38 di
	hint_nop38 word [0x9d5]
	hint_nop38 dword [0x33a]
	hint_nop38 ecx
	hint_nop38 rcx
	hint_nop38 rsi

%ifdef ERROR
	hint_nop38 r13w
	hint_nop38 r9d
	hint_nop38 r10
	hint_nop38 r23w
	hint_nop38 r20d
	hint_nop38 r31
%endif
