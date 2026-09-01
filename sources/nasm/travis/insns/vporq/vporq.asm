default rel
	vporq xmm2, xmm2, oword [0x2ad]
	vporq xmm2, oword [0x2ad]
	vporq xmm2, xmm0, xmm7
	vporq ymm2, ymm0, ymm0
	vporq ymm2, ymm0
	vporq ymm3, ymm3, ymm5
	vporq zmm2, zmm2, zword [0x49a]
	vporq zmm2, zword [0x49a]
	vporq zmm1, zmm3, zmm6
	vporq xmm15, xmm10, xmm15
	vporq ymm15, ymm14, ymm9
	vporq zmm8, zmm8, zmm13
	vporq xmm27, xmm26, xmm29
	vporq ymm18, ymm24, ymm25
	vporq zmm29, zmm20, zmm25
	vporq xmm6{k6}, xmm1, xmm5
	vporq ymm1{k6}, ymm4, ymm3
	vporq zmm1{k4}, zmm7, zword [0x86d]
	vporq xmm7{k6}{z}, xmm2, xmm4
	vporq ymm7{k2}{z}, ymm5, yword [0xfb9]
	vporq zmm4{k6}{z}, zmm1, zmm6
	vporq xmm6, xmm3, oword [eax+1]
	vporq xmm7, xmm2, oword [eax+64]
	vporq ymm1, ymm4, yword [eax+1]
	vporq ymm4, ymm0, yword [eax+64]
	vporq zmm4, zmm1, zword [eax+1]
	vporq zmm7, zmm6, zword [eax+64]
	vporq xmm0, xmm7, [0x3f5]
	vporq ymm0, ymm0, [0x313]
	vporq zmm6, zmm6, [0x163]
