	pcmpgtb mm4, mm7
	pcmpgtb mm7, mm2
	pcmpgtb xmm6, xmm3
	pcmpgtb xmm5, oword [0xe9b]

%ifdef ERROR
	pcmpgtb xmm8, xmm10
%endif
