	hint_nop61 bx
	hint_nop61 dx
	hint_nop61 eax
	hint_nop61 ecx
	hint_nop61 rdx
	hint_nop61 rsi

%ifdef ERROR
	hint_nop61 r11w
	hint_nop61 r13d
	hint_nop61 r12
	hint_nop61 r24w
	hint_nop61 r24d
	hint_nop61 r20
%endif
