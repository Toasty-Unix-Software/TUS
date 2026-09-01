	vpmacswd xmm2, xmm0, xmm3, xmm1
	vpmacswd xmm2, xmm3, xmm1
	vpmacswd xmm6, xmm2, xmm4, xmm5

%ifdef ERROR
	vpmacswd xmm14, xmm10, xmm8, xmm15
%endif
