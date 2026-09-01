	invpcid edx, oword [0xdfa]
	invpcid eax, oword [0x18b]

%ifdef ERROR
	invpcid rsi, oword [0xbad]
	invpcid rax, oword [0xffc]
%endif
