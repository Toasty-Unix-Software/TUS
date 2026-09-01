	vpblendvb xmm7, xmm5, xmm4, xmm6
	vpblendvb xmm7, xmm4, xmm6
	vpblendvb xmm1, xmm2, oword [0xa4c], xmm2
	vpblendvb ymm2, ymm6, ymm0, ymm4
	vpblendvb ymm2, ymm0, ymm4
	vpblendvb ymm3, ymm0, ymm4, ymm3

%ifdef ERROR
	vpblendvb xmm12, xmm15, xmm14, xmm8
	vpblendvb ymm8, ymm14, ymm8, ymm11
%endif
