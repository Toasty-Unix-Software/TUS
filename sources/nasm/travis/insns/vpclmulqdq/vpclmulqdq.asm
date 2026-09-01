default rel
	vpclmulqdq xmm5, xmm5, oword [0x6f2], 0x3b
	vpclmulqdq xmm5, oword [0x6f2], 0x3b
	vpclmulqdq xmm6, xmm7, oword [0xcbc], 0xd9
	vpclmulqdq ymm7, ymm7, ymm4, 0xc
	vpclmulqdq ymm7, ymm4, 0xc
	vpclmulqdq ymm2, ymm6, yword [0xe6c], 0x93
	vpclmulqdq zmm2, zmm1, zmm0, 0xb1
	vpclmulqdq zmm2, zmm0, 0xb1
	vpclmulqdq zmm6, zmm1, zmm3, 0x1a
	vpclmulqdq xmm9, xmm12, xmm9, 0x2c
	vpclmulqdq ymm10, ymm8, ymm14, 0xa0
	vpclmulqdq zmm10, zmm10, zmm8, 0xfb
	vpclmulqdq xmm30, xmm27, xmm23, 0xbf
	vpclmulqdq ymm21, ymm25, ymm21, 0xda
	vpclmulqdq zmm25, zmm17, zmm29, 0x5b
	vpclmulqdq xmm1, xmm5, oword [eax+1], 0x4f
	vpclmulqdq xmm6, xmm6, oword [eax+64], 0x3b
	vpclmulqdq ymm1, ymm6, yword [eax+1], 0xe4
	vpclmulqdq ymm3, ymm5, yword [eax+64], 0x93
	vpclmulqdq zmm6, zmm0, zword [eax+1], 0xc7
	vpclmulqdq zmm3, zmm6, zword [eax+64], 0x55
	vpclmulqdq xmm3, xmm7, [0xa16], 0xa4
	vpclmulqdq ymm7, ymm1, [0xa42], 0x1a
	vpclmulqdq zmm2, zmm5, [0x3bb], 0xf5
