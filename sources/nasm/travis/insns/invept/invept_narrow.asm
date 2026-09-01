	invept eax, [0x3e5]
	invept esi, [0xa5e]

%ifdef ERROR
	invept rsi, [0xd6d]
	invept rax, [0x6f4]
	invept rbx, oword [0xd90]
	invept rsi, oword [0x439]
%endif
