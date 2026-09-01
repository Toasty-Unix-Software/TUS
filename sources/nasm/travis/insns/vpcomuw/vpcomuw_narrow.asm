	vpcomuw xmm6, xmm2, xmm5, 0xe5
	vpcomuw xmm6, xmm5, 0xe5
	vpcomuw xmm0, xmm4, xmm1, 0xc6

%ifdef ERROR
	vpcomuw xmm15, xmm8, xmm9, 0xb4
%endif
