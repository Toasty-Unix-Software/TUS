	vpclmulhqhqdq xmm2, xmm0, xmm6
	vpclmulhqhqdq xmm2, xmm6
	vpclmulhqhqdq xmm3, xmm0, oword [0xf1d]
	vpclmulhqhqdq ymm5, ymm7, yword [0x1f5]
	vpclmulhqhqdq ymm5, yword [0x1f5]
	vpclmulhqhqdq ymm1, ymm3, yword [0x842]
	vpclmulhqhqdq zmm6, zmm3, zmm6
	vpclmulhqhqdq zmm6, zmm6
	vpclmulhqhqdq zmm7, zmm7, zmm0

%ifdef ERROR
	vpclmulhqhqdq xmm13, xmm14, xmm13
	vpclmulhqhqdq ymm15, ymm11, ymm12
	vpclmulhqhqdq zmm11, zmm12, zmm13
	vpclmulhqhqdq xmm25, xmm30, xmm19
	vpclmulhqhqdq ymm30, ymm26, ymm27
	vpclmulhqhqdq zmm31, zmm17, zmm19
%endif
