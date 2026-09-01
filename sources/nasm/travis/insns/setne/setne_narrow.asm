	setne byte [0x598]
	setne al
	setne ebp
	setne eax

%ifdef ERROR
	setne rbp
	setne rbp
	setne r11b
	setne r10
	setne r9d
	setne r22b
	setne r17
	setne r30d
%endif
