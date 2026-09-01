	vphadduwq xmm1, oword [0xa17]
	vphadduwq xmm1
	vphadduwq xmm4, xmm3

%ifdef ERROR
	vphadduwq xmm12, xmm15
%endif
