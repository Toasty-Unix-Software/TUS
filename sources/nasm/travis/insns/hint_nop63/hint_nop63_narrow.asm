	hint_nop63 cx
	hint_nop63 di
	hint_nop63 ebx
	hint_nop63 edi
	hint_nop63 rsi
	hint_nop63 rcx

%ifdef ERROR
	hint_nop63 r8w
	hint_nop63 r10d
	hint_nop63 r15
	hint_nop63 r18w
	hint_nop63 r24d
	hint_nop63 r27
%endif
