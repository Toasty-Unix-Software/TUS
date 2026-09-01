	hint_nop16 cx
	hint_nop16 dx
	hint_nop16 dword [0xe9a]
	hint_nop16 ecx
	hint_nop16 rsi
	hint_nop16 rbp

%ifdef ERROR
	hint_nop16 r14w
	hint_nop16 r9d
	hint_nop16 r15
	hint_nop16 r19w
	hint_nop16 r18d
	hint_nop16 r17
%endif
