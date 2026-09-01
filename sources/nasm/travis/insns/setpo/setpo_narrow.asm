	setpo byte [0xfbc]
	setpo byte [0xa44]
	setpo ebp
	setpo esi

%ifdef ERROR
	setpo rdi
	setpo rdx
	setpo r13b
	setpo r8
	setpo r10d
	setpo r22b
	setpo r31
	setpo r28d
%endif
