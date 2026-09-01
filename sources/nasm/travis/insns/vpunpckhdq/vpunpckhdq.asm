default rel
	vpunpckhdq xmm4, xmm1, xmm0
	vpunpckhdq xmm4, xmm0
	vpunpckhdq xmm1, xmm4, xmm5
	vpunpckhdq ymm6, ymm3, yword [0x7a3]
	vpunpckhdq ymm6, yword [0x7a3]
	vpunpckhdq ymm5, ymm0, ymm2
	vpunpckhdq xmm5, xmm6, xmm4
	vpunpckhdq xmm5, xmm4
	vpunpckhdq xmm3, xmm0, xmm6
	vpunpckhdq ymm1, ymm0, ymm7
	vpunpckhdq ymm1, ymm7
	vpunpckhdq ymm0, ymm2, ymm6
	vpunpckhdq xmm9, xmm15, xmm13
	vpunpckhdq ymm12, ymm12, ymm9
	vpunpckhdq xmm9, xmm14, xmm8
	vpunpckhdq ymm8, ymm10, ymm12
	vpunpckhdq xmm16, xmm31, xmm27
	vpunpckhdq ymm20, ymm19, ymm17
	vpunpckhdq xmm30, xmm23, xmm31
	vpunpckhdq ymm26, ymm24, ymm18
	vpunpckhdq xmm7{k7}, xmm1, xmm7
	vpunpckhdq ymm0{k4}, ymm7, ymm2
	vpunpckhdq zmm7{k5}, zmm2, zmm3
	vpunpckhdq xmm4{k7}{z}, xmm3, xmm2
	vpunpckhdq ymm6{k2}{z}, ymm0, yword [0xf16]
	vpunpckhdq zmm1{k2}{z}, zmm3, zword [0xf1f]
	vpunpckhdq xmm5, xmm6, oword [eax+1]
	vpunpckhdq xmm0, xmm6, oword [eax+64]
	vpunpckhdq ymm3, ymm1, yword [eax+1]
	vpunpckhdq ymm5, ymm1, yword [eax+64]
	vpunpckhdq xmm2, xmm6, oword [eax+1]
	vpunpckhdq xmm7, xmm4, oword [eax+64]
	vpunpckhdq ymm3, ymm6, yword [eax+1]
	vpunpckhdq ymm2, ymm6, yword [eax+64]
	vpunpckhdq zmm0, zmm2, zword [eax+1]
	vpunpckhdq zmm5, zmm5, zword [eax+64]
	vpunpckhdq xmm3, xmm3, [0x9af]
	vpunpckhdq ymm0, ymm6, [0x2ad]
	vpunpckhdq xmm5, xmm6, [0xbb9]
	vpunpckhdq ymm5, ymm0, [0x532]
	vpunpckhdq zmm0, zmm2, [0xb68]
