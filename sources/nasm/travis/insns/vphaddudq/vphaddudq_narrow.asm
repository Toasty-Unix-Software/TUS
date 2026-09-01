	vphaddudq xmm1, oword [0x449]
	vphaddudq xmm1
	vphaddudq xmm6, xmm2

%ifdef ERROR
	vphaddudq xmm11, xmm10
%endif
