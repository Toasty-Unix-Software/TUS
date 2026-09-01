	hint_nop40 ax
	hint_nop40 di
	hint_nop40 ebx
	hint_nop40 eax
	hint_nop40 rdi
	hint_nop40 rbx

%ifdef ERROR
	hint_nop40 r13w
	hint_nop40 r13d
	hint_nop40 r8
	hint_nop40 r22w
	hint_nop40 r23d
	hint_nop40 r17
%endif
