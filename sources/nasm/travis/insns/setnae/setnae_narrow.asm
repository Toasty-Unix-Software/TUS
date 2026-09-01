	setnae bl
	setnae byte [0x14b]
	setnae ecx
	setnae edx

%ifdef ERROR
	setnae rsi
	setnae rdx
	setnae r15b
	setnae r10
	setnae r9d
	setnae r20b
	setnae r28
	setnae r18d
%endif
