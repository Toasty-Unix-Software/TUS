	setozu ebx
	setozu edi
	setozu byte [0x30b]
	setozu byte [0x6b0]

%ifdef ERROR
	setozu rbp
	setozu rax
	setozu r12
	setozu r15d
	setozu r15b
	setozu r31
	setozu r19d
	setozu r24b
%endif
