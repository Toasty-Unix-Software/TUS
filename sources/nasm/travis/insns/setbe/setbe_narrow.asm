	setbe byte [0xb09]
	setbe al
	setbe ecx
	setbe eax

%ifdef ERROR
	setbe rbp
	setbe rbx
	setbe r12b
	setbe r14
	setbe r14d
	setbe r31b
	setbe r24
	setbe r21d
%endif
