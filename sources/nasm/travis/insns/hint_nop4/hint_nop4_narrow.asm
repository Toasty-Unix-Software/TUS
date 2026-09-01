	hint_nop4 word [0x63c]
	hint_nop4 bx
	hint_nop4 edx
	hint_nop4 ecx
	hint_nop4 rcx
	hint_nop4 rcx

%ifdef ERROR
	hint_nop4 r9w
	hint_nop4 r15d
	hint_nop4 r14
	hint_nop4 r21w
	hint_nop4 r28d
	hint_nop4 r24
%endif
