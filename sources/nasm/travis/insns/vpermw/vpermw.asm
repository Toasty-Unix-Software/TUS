default rel
	vpermw xmm1, xmm0, xmm2
	vpermw xmm1, xmm2
	vpermw xmm3, xmm7, xmm0
	vpermw ymm4, ymm6, yword [0x769]
	vpermw ymm4, yword [0x769]
	vpermw ymm5, ymm4, ymm0
	vpermw zmm7, zmm4, zmm1
	vpermw zmm7, zmm1
	vpermw zmm0, zmm4, zmm2
	vpermw xmm14, xmm10, xmm9
	vpermw ymm13, ymm12, ymm15
	vpermw zmm11, zmm8, zmm12
	vpermw xmm17, xmm24, xmm17
	vpermw ymm24, ymm17, ymm29
	vpermw zmm29, zmm30, zmm24
	vpermw xmm3{k7}, xmm4, oword [0xc6f]
	vpermw ymm1{k4}, ymm6, ymm2
	vpermw zmm3{k6}, zmm7, zmm5
	vpermw xmm5{k4}{z}, xmm7, xmm7
	vpermw ymm2{k2}{z}, ymm1, ymm0
	vpermw zmm2{k1}{z}, zmm7, zword [0xe45]
	vpermw xmm4, xmm1, oword [eax+1]
	vpermw xmm5, xmm6, oword [eax+64]
	vpermw ymm6, ymm3, yword [eax+1]
	vpermw ymm0, ymm3, yword [eax+64]
	vpermw zmm4, zmm3, zword [eax+1]
	vpermw zmm4, zmm6, zword [eax+64]
	vpermw xmm2, xmm1, [0x45a]
	vpermw ymm6, ymm0, [0x6b4]
	vpermw zmm2, zmm7, [0xa3b]
