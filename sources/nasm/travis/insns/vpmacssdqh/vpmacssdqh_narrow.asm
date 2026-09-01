	vpmacssdqh xmm1, xmm4, xmm2, xmm7
	vpmacssdqh xmm1, xmm2, xmm7
	vpmacssdqh xmm0, xmm5, oword [0x26f], xmm3

%ifdef ERROR
	vpmacssdqh xmm10, xmm12, xmm8, xmm14
%endif
