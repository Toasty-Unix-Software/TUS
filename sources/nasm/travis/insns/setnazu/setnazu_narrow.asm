	setnazu eax
	setnazu ecx
	setnazu byte [0x9cd]
	setnazu bl

%ifdef ERROR
	setnazu rax
	setnazu rbx
	setnazu r15
	setnazu r11d
	setnazu r14b
	setnazu r26
	setnazu r16d
	setnazu r16b
%endif
