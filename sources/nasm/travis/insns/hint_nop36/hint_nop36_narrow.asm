	hint_nop36 ax
	hint_nop36 di
	hint_nop36 dword [0x3fe]
	hint_nop36 ebx
	hint_nop36 rsi
	hint_nop36 qword [0x84d]

%ifdef ERROR
	hint_nop36 r8w
	hint_nop36 r8d
	hint_nop36 r9
	hint_nop36 r20w
	hint_nop36 r28d
	hint_nop36 r28
%endif
