default rel
	vpaddq xmm1, xmm2, xmm6
	vpaddq xmm1, xmm6
	vpaddq xmm2, xmm6, oword [0xdc5]
	vpaddq ymm2, ymm4, ymm1
	vpaddq ymm2, ymm1
	vpaddq ymm3, ymm6, ymm7
	vpaddq xmm6, xmm6, xmm2
	vpaddq xmm6, xmm2
	vpaddq xmm4, xmm7, xmm0
	vpaddq ymm1, ymm6, ymm5
	vpaddq ymm1, ymm5
	vpaddq ymm6, ymm1, ymm6
	vpaddq xmm14, xmm13, xmm13
	vpaddq ymm8, ymm12, ymm10
	vpaddq xmm12, xmm15, xmm15
	vpaddq ymm13, ymm13, ymm8
	vpaddq xmm25, xmm18, xmm30
	vpaddq ymm28, ymm23, ymm24
	vpaddq xmm30, xmm20, xmm27
	vpaddq ymm17, ymm25, ymm24
	vpaddq xmm5{k1}, xmm3, xmm7
	vpaddq ymm4{k6}, ymm0, ymm6
	vpaddq zmm7{k2}, zmm7, zmm1
	vpaddq xmm3{k4}{z}, xmm7, xmm5
	vpaddq ymm1{k2}{z}, ymm7, yword [0x7af]
	vpaddq zmm0{k4}{z}, zmm3, zword [0xe69]
	vpaddq xmm5, xmm4, oword [eax+1]
	vpaddq xmm4, xmm0, oword [eax+64]
	vpaddq ymm1, ymm3, yword [eax+1]
	vpaddq ymm3, ymm2, yword [eax+64]
	vpaddq xmm2, xmm3, oword [eax+1]
	vpaddq xmm0, xmm7, oword [eax+64]
	vpaddq ymm0, ymm7, yword [eax+1]
	vpaddq ymm5, ymm7, yword [eax+64]
	vpaddq zmm3, zmm6, zword [eax+1]
	vpaddq zmm1, zmm5, zword [eax+64]
	vpaddq xmm7, xmm7, [0xe3f]
	vpaddq ymm7, ymm5, [0xc4a]
	vpaddq xmm5, xmm6, [0xf47]
	vpaddq ymm1, ymm6, [0x107]
	vpaddq zmm5, zmm1, [0x38d]
