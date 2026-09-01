	vpmacsdd xmm2, xmm3, xmm2, xmm0
	vpmacsdd xmm2, xmm2, xmm0
	vpmacsdd xmm7, xmm0, xmm4, xmm2

%ifdef ERROR
	vpmacsdd xmm8, xmm12, xmm14, xmm8
%endif
