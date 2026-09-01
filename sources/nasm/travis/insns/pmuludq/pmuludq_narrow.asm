	pmuludq mm2, mm3
	pmuludq mm1, mm4
	pmuludq xmm2, xmm6
	pmuludq xmm1, xmm2

%ifdef ERROR
	pmuludq xmm11, xmm8
%endif
