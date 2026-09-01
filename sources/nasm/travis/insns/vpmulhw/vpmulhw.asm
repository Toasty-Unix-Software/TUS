default rel
	vpmulhw xmm6, xmm3, oword [0x190]
	vpmulhw xmm6, oword [0x190]
	vpmulhw xmm0, xmm6, oword [0x1e1]
	vpmulhw ymm3, ymm6, ymm0
	vpmulhw ymm3, ymm0
	vpmulhw ymm0, ymm3, yword [0xd73]
	vpmulhw xmm0, xmm0, oword [0x932]
	vpmulhw xmm0, oword [0x932]
	vpmulhw xmm6, xmm4, xmm1
	vpmulhw ymm3, ymm7, yword [0xfea]
	vpmulhw ymm3, yword [0xfea]
	vpmulhw ymm0, ymm6, ymm4
	vpmulhw xmm15, xmm11, xmm10
	vpmulhw ymm14, ymm14, ymm14
	vpmulhw xmm9, xmm12, xmm11
	vpmulhw ymm12, ymm10, ymm10
	vpmulhw xmm25, xmm20, xmm25
	vpmulhw ymm20, ymm21, ymm20
	vpmulhw xmm19, xmm29, xmm23
	vpmulhw ymm17, ymm22, ymm30
	vpmulhw xmm4{k4}, xmm6, xmm7
	vpmulhw ymm7{k5}, ymm4, ymm0
	vpmulhw zmm3{k5}, zmm6, zmm0
	vpmulhw xmm3{k4}{z}, xmm3, xmm0
	vpmulhw ymm5{k3}{z}, ymm0, ymm3
	vpmulhw zmm4{k4}{z}, zmm5, zword [0x6d8]
	vpmulhw xmm5, xmm5, oword [eax+1]
	vpmulhw xmm5, xmm3, oword [eax+64]
	vpmulhw ymm2, ymm4, yword [eax+1]
	vpmulhw ymm0, ymm5, yword [eax+64]
	vpmulhw xmm6, xmm2, oword [eax+1]
	vpmulhw xmm0, xmm0, oword [eax+64]
	vpmulhw ymm5, ymm4, yword [eax+1]
	vpmulhw ymm5, ymm0, yword [eax+64]
	vpmulhw zmm3, zmm7, zword [eax+1]
	vpmulhw zmm7, zmm2, zword [eax+64]
	vpmulhw xmm2, xmm2, [0xba4]
	vpmulhw ymm6, ymm3, [0x8d6]
	vpmulhw xmm1, xmm4, [0xdfc]
	vpmulhw ymm5, ymm6, [0xe6d]
	vpmulhw zmm0, zmm3, [0xdaf]
