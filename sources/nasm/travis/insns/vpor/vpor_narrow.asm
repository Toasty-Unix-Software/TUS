	vpor xmm7, xmm3, xmm6
	vpor xmm7, xmm6
	vpor xmm3, xmm1, oword [0xef2]
	vpor ymm1, ymm6, ymm1
	vpor ymm1, ymm1
	vpor ymm2, ymm0, ymm7

%ifdef ERROR
	vpor xmm9, xmm10, xmm10
	vpor ymm12, ymm11, ymm9
%endif
