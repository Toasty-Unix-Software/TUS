	vpxor xmm5, xmm5, xmm3
	vpxor xmm5, xmm3
	vpxor xmm0, xmm0, xmm1
	vpxor ymm7, ymm2, ymm6
	vpxor ymm7, ymm6
	vpxor ymm2, ymm6, yword [0x77d]

%ifdef ERROR
	vpxor xmm15, xmm8, xmm9
	vpxor ymm12, ymm14, ymm10
%endif
