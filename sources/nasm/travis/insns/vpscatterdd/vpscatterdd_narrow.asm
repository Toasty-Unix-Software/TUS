	vpscatterdd [xmm2*1], xmm5
	vpscatterdd [xmm6*1], xmm2
	vpscatterdd [ymm3*1], ymm3
	vpscatterdd [ymm3*1], ymm3
	vpscatterdd [zmm7*1], zmm1
	vpscatterdd [zmm5*1], zmm1

%ifdef ERROR
	vpscatterdd [xmm14*1], xmm9
	vpscatterdd [ymm12*1], ymm15
	vpscatterdd [zmm9*1], zmm10
	vpscatterdd [xmm23*1], xmm26
	vpscatterdd [ymm30*1], ymm23
	vpscatterdd [zmm30*1], zmm25
%endif
