	setngezu edx
	setngezu edi
	setngezu byte [0xd7f]
	setngezu bl

%ifdef ERROR
	setngezu rbp
	setngezu rbx
	setngezu r14
	setngezu r11d
	setngezu r13b
	setngezu r22
	setngezu r28d
	setngezu r20b
%endif
