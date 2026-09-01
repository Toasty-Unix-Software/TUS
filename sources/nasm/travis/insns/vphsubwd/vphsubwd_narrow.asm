	vphsubwd xmm1, xmm3
	vphsubwd xmm1
	vphsubwd xmm2, xmm0

%ifdef ERROR
	vphsubwd xmm14, xmm15
%endif
