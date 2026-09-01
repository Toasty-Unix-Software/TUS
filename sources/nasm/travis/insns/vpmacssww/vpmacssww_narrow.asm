	vpmacssww xmm0, xmm4, xmm2, xmm4
	vpmacssww xmm0, xmm2, xmm4
	vpmacssww xmm7, xmm2, xmm7, xmm3

%ifdef ERROR
	vpmacssww xmm15, xmm15, xmm10, xmm12
%endif
