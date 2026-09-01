	setns byte [0x154]
	setns byte [0xcf3]
	setns ecx
	setns edx

%ifdef ERROR
	setns rdx
	setns rbp
	setns r8b
	setns r15
	setns r10d
	setns r28b
	setns r27
	setns r31d
%endif
