default rel
	vpor xmm7, xmm3, xmm6
	vpor xmm7, xmm6
	vpor xmm3, xmm1, oword [0xef2]
	vpor ymm1, ymm6, ymm1
	vpor ymm1, ymm1
	vpor ymm2, ymm0, ymm7
	vpor xmm9, xmm10, xmm10
	vpor ymm12, ymm11, ymm9
	vpor xmm3, xmm6, oword [eax+1]
	vpor xmm6, xmm6, oword [eax+64]
	vpor ymm7, ymm5, yword [eax+1]
	vpor ymm6, ymm7, yword [eax+64]
	vpor xmm1, xmm1, [0x66f]
	vpor ymm4, ymm5, [0x95c]
