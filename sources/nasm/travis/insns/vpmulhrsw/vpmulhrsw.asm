default rel
	vpmulhrsw xmm7, xmm5, oword [0x29f]
	vpmulhrsw xmm7, oword [0x29f]
	vpmulhrsw xmm5, xmm2, xmm7
	vpmulhrsw ymm3, ymm5, ymm7
	vpmulhrsw ymm3, ymm7
	vpmulhrsw ymm4, ymm0, yword [0xfd1]
	vpmulhrsw xmm2, xmm1, xmm0
	vpmulhrsw xmm2, xmm0
	vpmulhrsw xmm3, xmm3, oword [0x79a]
	vpmulhrsw ymm7, ymm1, ymm4
	vpmulhrsw ymm7, ymm4
	vpmulhrsw ymm7, ymm4, ymm0
	vpmulhrsw xmm8, xmm11, xmm15
	vpmulhrsw ymm10, ymm10, ymm14
	vpmulhrsw xmm13, xmm11, xmm9
	vpmulhrsw ymm13, ymm14, ymm15
	vpmulhrsw xmm31, xmm30, xmm28
	vpmulhrsw ymm25, ymm20, ymm28
	vpmulhrsw xmm21, xmm28, xmm23
	vpmulhrsw ymm24, ymm17, ymm18
	vpmulhrsw xmm2{k5}, xmm0, oword [0xb04]
	vpmulhrsw ymm0{k6}, ymm6, ymm7
	vpmulhrsw zmm4{k3}, zmm2, zmm3
	vpmulhrsw xmm5{k2}{z}, xmm4, oword [0xc58]
	vpmulhrsw ymm7{k5}{z}, ymm6, yword [0xa9d]
	vpmulhrsw zmm6{k2}{z}, zmm6, zword [0xff9]
	vpmulhrsw xmm6, xmm1, oword [eax+1]
	vpmulhrsw xmm1, xmm0, oword [eax+64]
	vpmulhrsw ymm1, ymm2, yword [eax+1]
	vpmulhrsw ymm6, ymm5, yword [eax+64]
	vpmulhrsw xmm0, xmm3, oword [eax+1]
	vpmulhrsw xmm6, xmm2, oword [eax+64]
	vpmulhrsw ymm5, ymm2, yword [eax+1]
	vpmulhrsw ymm2, ymm2, yword [eax+64]
	vpmulhrsw zmm4, zmm4, zword [eax+1]
	vpmulhrsw zmm3, zmm5, zword [eax+64]
	vpmulhrsw xmm3, xmm5, [0x5e6]
	vpmulhrsw ymm5, ymm1, [0x60a]
	vpmulhrsw xmm3, xmm7, [0xf55]
	vpmulhrsw ymm0, ymm2, [0x3a1]
	vpmulhrsw zmm7, zmm6, [0xe36]
