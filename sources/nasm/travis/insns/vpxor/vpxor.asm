default rel
	vpxor xmm5, xmm5, xmm3
	vpxor xmm5, xmm3
	vpxor xmm0, xmm0, xmm1
	vpxor ymm7, ymm2, ymm6
	vpxor ymm7, ymm6
	vpxor ymm2, ymm6, yword [0x77d]
	vpxor xmm15, xmm8, xmm9
	vpxor ymm12, ymm14, ymm10
	vpxor xmm1, xmm2, oword [eax+1]
	vpxor xmm0, xmm3, oword [eax+64]
	vpxor ymm1, ymm4, yword [eax+1]
	vpxor ymm4, ymm0, yword [eax+64]
	vpxor xmm1, xmm5, [0x33c]
	vpxor ymm7, ymm0, [0xf81]
