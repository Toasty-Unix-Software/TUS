	sldt word [0xabb]
	sldt word [0x27c]
	sldt dx
	sldt di
	sldt ebp
	sldt eax

%ifdef ERROR
	sldt rdi
	sldt rdx
	sldt r15w
	sldt r15d
	sldt r12
	sldt r26w
	sldt r19d
	sldt r28
%endif
