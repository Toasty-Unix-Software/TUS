	vphaddwd xmm3, xmm4
	vphaddwd xmm3
	vphaddwd xmm4, xmm7

%ifdef ERROR
	vphaddwd xmm10, xmm9
%endif
