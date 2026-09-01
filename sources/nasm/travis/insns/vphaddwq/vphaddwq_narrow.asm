	vphaddwq xmm1, oword [0x356]
	vphaddwq xmm1
	vphaddwq xmm7, xmm1

%ifdef ERROR
	vphaddwq xmm11, xmm15
%endif
