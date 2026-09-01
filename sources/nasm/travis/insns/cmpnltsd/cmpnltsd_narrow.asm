	cmpnltsd xmm6, xmm7
	cmpnltsd xmm7, oword [0x6c4]

%ifdef ERROR
	cmpnltsd xmm11, xmm15
%endif
