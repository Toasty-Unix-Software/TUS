	setezu ebp
	setezu edx
	setezu bl
	setezu byte [0x2f3]

%ifdef ERROR
	setezu rbp
	setezu rbx
	setezu r13
	setezu r15d
	setezu r8b
	setezu r28
	setezu r19d
	setezu r24b
%endif
