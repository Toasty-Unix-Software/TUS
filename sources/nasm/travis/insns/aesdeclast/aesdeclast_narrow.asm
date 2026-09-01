	aesdeclast xmm6, xmm7
	aesdeclast xmm6, oword [0x5f8]

%ifdef ERROR
	aesdeclast xmm13, xmm9
%endif
