	vpclmulhqlqdq xmm3, xmm1, oword [0xbae]
	vpclmulhqlqdq xmm3, oword [0xbae]
	vpclmulhqlqdq xmm0, xmm6, oword [0x10c]
	vpclmulhqlqdq ymm6, ymm1, ymm1
	vpclmulhqlqdq ymm6, ymm1
	vpclmulhqlqdq ymm2, ymm0, ymm4
	vpclmulhqlqdq zmm0, zmm4, zmm1
	vpclmulhqlqdq zmm0, zmm1
	vpclmulhqlqdq zmm2, zmm3, zmm0

%ifdef ERROR
	vpclmulhqlqdq xmm12, xmm9, xmm13
	vpclmulhqlqdq ymm9, ymm12, ymm14
	vpclmulhqlqdq zmm12, zmm13, zmm12
	vpclmulhqlqdq xmm21, xmm26, xmm22
	vpclmulhqlqdq ymm24, ymm23, ymm30
	vpclmulhqlqdq zmm25, zmm23, zmm28
%endif
