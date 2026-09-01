	setlzu eax
	setlzu esi
	setlzu byte [0x69f]
	setlzu byte [0xf8d]

%ifdef ERROR
	setlzu rbx
	setlzu rsi
	setlzu r12
	setlzu r13d
	setlzu r13b
	setlzu r25
	setlzu r24d
	setlzu r16b
%endif
