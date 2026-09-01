	setno bl
	setno byte [0xeeb]
	setno ebx
	setno ebp

%ifdef ERROR
	setno rsi
	setno rcx
	setno r15b
	setno r10
	setno r14d
	setno r18b
	setno r22
	setno r21d
%endif
