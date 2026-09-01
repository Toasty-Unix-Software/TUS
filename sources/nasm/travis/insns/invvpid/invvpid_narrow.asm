	invvpid edx, [0x669]
	invvpid edi, [0xb93]

%ifdef ERROR
	invvpid rax, [0xfba]
	invvpid rax, [0xc2a]
	invvpid rsi, oword [0x564]
	invvpid rdx, oword [0x4d1]
%endif
