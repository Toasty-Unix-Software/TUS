	setz byte [0x8b0]
	setz byte [0x881]
	setz edx
	setz ecx

%ifdef ERROR
	setz rsi
	setz rbp
	setz r14b
	setz r15
	setz r13d
	setz r29b
	setz r22
	setz r24d
%endif
