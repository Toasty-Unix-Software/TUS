	setgezu edi
	setgezu ebx
	setgezu byte [0x25a]
	setgezu dl

%ifdef ERROR
	setgezu rax
	setgezu rsi
	setgezu r15
	setgezu r10d
	setgezu r8b
	setgezu r30
	setgezu r26d
	setgezu r22b
%endif
