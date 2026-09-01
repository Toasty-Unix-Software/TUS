default rel
	vpandnq xmm4, xmm3, xmm3
	vpandnq xmm4, xmm3
	vpandnq xmm3, xmm7, oword [0xb4b]
	vpandnq ymm0, ymm6, ymm4
	vpandnq ymm0, ymm4
	vpandnq ymm0, ymm7, ymm4
	vpandnq zmm6, zmm2, zmm7
	vpandnq zmm6, zmm7
	vpandnq zmm7, zmm4, zmm1
	vpandnq xmm11, xmm9, xmm11
	vpandnq ymm13, ymm15, ymm15
	vpandnq zmm8, zmm15, zmm10
	vpandnq xmm24, xmm22, xmm30
	vpandnq ymm30, ymm24, ymm30
	vpandnq zmm17, zmm25, zmm20
	vpandnq xmm5{k3}, xmm0, xmm6
	vpandnq ymm7{k3}, ymm3, ymm7
	vpandnq zmm7{k1}, zmm2, zmm2
	vpandnq xmm0{k6}{z}, xmm7, xmm5
	vpandnq ymm4{k2}{z}, ymm5, ymm1
	vpandnq zmm4{k2}{z}, zmm5, zmm0
	vpandnq xmm1, xmm6, oword [eax+1]
	vpandnq xmm2, xmm1, oword [eax+64]
	vpandnq ymm7, ymm6, yword [eax+1]
	vpandnq ymm1, ymm1, yword [eax+64]
	vpandnq zmm7, zmm0, zword [eax+1]
	vpandnq zmm3, zmm6, zword [eax+64]
	vpandnq xmm3, xmm6, [0x1b9]
	vpandnq ymm6, ymm1, [0x143]
	vpandnq zmm5, zmm1, [0x953]
