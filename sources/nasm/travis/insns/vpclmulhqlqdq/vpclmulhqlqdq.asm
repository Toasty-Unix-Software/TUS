default rel
	vpclmulhqlqdq xmm3, xmm1, oword [0xbae]
	vpclmulhqlqdq xmm3, oword [0xbae]
	vpclmulhqlqdq xmm0, xmm6, oword [0x10c]
	vpclmulhqlqdq ymm6, ymm1, ymm1
	vpclmulhqlqdq ymm6, ymm1
	vpclmulhqlqdq ymm2, ymm0, ymm4
	vpclmulhqlqdq zmm0, zmm4, zmm1
	vpclmulhqlqdq zmm0, zmm1
	vpclmulhqlqdq zmm2, zmm3, zmm0
	vpclmulhqlqdq xmm12, xmm9, xmm13
	vpclmulhqlqdq ymm9, ymm12, ymm14
	vpclmulhqlqdq zmm12, zmm13, zmm12
	vpclmulhqlqdq xmm21, xmm26, xmm22
	vpclmulhqlqdq ymm24, ymm23, ymm30
	vpclmulhqlqdq zmm25, zmm23, zmm28
	vpclmulhqlqdq xmm7, xmm4, oword [eax+1]
	vpclmulhqlqdq xmm7, xmm2, oword [eax+64]
	vpclmulhqlqdq ymm2, ymm5, yword [eax+1]
	vpclmulhqlqdq ymm1, ymm3, yword [eax+64]
	vpclmulhqlqdq zmm6, zmm5, zword [eax+1]
	vpclmulhqlqdq zmm7, zmm2, zword [eax+64]
	vpclmulhqlqdq xmm3, xmm6, [0x614]
	vpclmulhqlqdq ymm5, ymm0, [0x203]
	vpclmulhqlqdq zmm0, zmm4, [0x439]
