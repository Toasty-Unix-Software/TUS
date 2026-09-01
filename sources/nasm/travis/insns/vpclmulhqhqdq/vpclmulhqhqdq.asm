default rel
	vpclmulhqhqdq xmm2, xmm0, xmm6
	vpclmulhqhqdq xmm2, xmm6
	vpclmulhqhqdq xmm3, xmm0, oword [0xf1d]
	vpclmulhqhqdq ymm5, ymm7, yword [0x1f5]
	vpclmulhqhqdq ymm5, yword [0x1f5]
	vpclmulhqhqdq ymm1, ymm3, yword [0x842]
	vpclmulhqhqdq zmm6, zmm3, zmm6
	vpclmulhqhqdq zmm6, zmm6
	vpclmulhqhqdq zmm7, zmm7, zmm0
	vpclmulhqhqdq xmm13, xmm14, xmm13
	vpclmulhqhqdq ymm15, ymm11, ymm12
	vpclmulhqhqdq zmm11, zmm12, zmm13
	vpclmulhqhqdq xmm25, xmm30, xmm19
	vpclmulhqhqdq ymm30, ymm26, ymm27
	vpclmulhqhqdq zmm31, zmm17, zmm19
	vpclmulhqhqdq xmm4, xmm5, oword [eax+1]
	vpclmulhqhqdq xmm6, xmm4, oword [eax+64]
	vpclmulhqhqdq ymm0, ymm7, yword [eax+1]
	vpclmulhqhqdq ymm3, ymm6, yword [eax+64]
	vpclmulhqhqdq zmm7, zmm1, zword [eax+1]
	vpclmulhqhqdq zmm3, zmm0, zword [eax+64]
	vpclmulhqhqdq xmm3, xmm3, [0xba5]
	vpclmulhqhqdq ymm0, ymm3, [0x782]
	vpclmulhqhqdq zmm0, zmm5, [0xa13]
