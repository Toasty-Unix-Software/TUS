	sete byte [0x4ef]
	sete byte [0xa24]
	sete edx
	sete eax

%ifdef ERROR
	sete rbp
	sete rax
	sete r15b
	sete r11
	sete r15d
	sete r18b
	sete r26
	sete r21d
%endif
