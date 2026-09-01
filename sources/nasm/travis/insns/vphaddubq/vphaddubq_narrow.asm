	vphaddubq xmm1, oword [0xa5e]
	vphaddubq xmm1
	vphaddubq xmm0, xmm7

%ifdef ERROR
	vphaddubq xmm9, xmm15
%endif
