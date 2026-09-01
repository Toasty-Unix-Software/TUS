	setnc cl
	setnc cl
	setnc ebx
	setnc ecx

%ifdef ERROR
	setnc rax
	setnc rdi
	setnc r15b
	setnc r12
	setnc r8d
	setnc r18b
	setnc r31
	setnc r20d
%endif
