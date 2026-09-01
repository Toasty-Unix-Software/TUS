	hint_nop5 ax
	hint_nop5 cx
	hint_nop5 edi
	hint_nop5 ecx
	hint_nop5 rdi
	hint_nop5 rbx

%ifdef ERROR
	hint_nop5 r9w
	hint_nop5 r9d
	hint_nop5 r11
	hint_nop5 r26w
	hint_nop5 r24d
	hint_nop5 r24
%endif
