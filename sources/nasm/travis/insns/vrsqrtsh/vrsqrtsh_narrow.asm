	vrsqrtsh xmm4, xmm0, xmm2
	vrsqrtsh xmm4, xmm2
	vrsqrtsh xmm4, xmm2, xmm4

%ifdef ERROR
	vrsqrtsh xmm11, xmm13, xmm11
	vrsqrtsh xmm19, xmm22, xmm31
%endif
