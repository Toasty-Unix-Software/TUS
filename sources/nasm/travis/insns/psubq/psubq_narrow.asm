	psubq mm4, mm3
	psubq mm4, mm3
	psubq xmm0, xmm4
	psubq xmm4, xmm4

%ifdef ERROR
	psubq xmm11, xmm11
%endif
