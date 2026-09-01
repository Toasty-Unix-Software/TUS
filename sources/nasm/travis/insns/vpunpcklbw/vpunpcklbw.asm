default rel
	vpunpcklbw xmm0, xmm3, oword [0x912]
	vpunpcklbw xmm0, oword [0x912]
	vpunpcklbw xmm6, xmm7, xmm1
	vpunpcklbw ymm0, ymm6, ymm6
	vpunpcklbw ymm0, ymm6
	vpunpcklbw ymm0, ymm1, yword [0x8ed]
	vpunpcklbw xmm1, xmm6, xmm4
	vpunpcklbw xmm1, xmm4
	vpunpcklbw xmm0, xmm4, oword [0x39d]
	vpunpcklbw ymm0, ymm7, ymm2
	vpunpcklbw ymm0, ymm2
	vpunpcklbw ymm7, ymm2, yword [0x370]
	vpunpcklbw xmm10, xmm8, xmm13
	vpunpcklbw ymm12, ymm13, ymm8
	vpunpcklbw xmm14, xmm12, xmm12
	vpunpcklbw ymm12, ymm11, ymm12
	vpunpcklbw xmm16, xmm23, xmm17
	vpunpcklbw ymm20, ymm24, ymm21
	vpunpcklbw xmm22, xmm19, xmm21
	vpunpcklbw ymm26, ymm29, ymm25
	vpunpcklbw xmm1{k3}, xmm0, oword [0x614]
	vpunpcklbw ymm4{k4}, ymm3, ymm4
	vpunpcklbw zmm4{k6}, zmm2, zmm7
	vpunpcklbw xmm6{k5}{z}, xmm1, oword [0xbc3]
	vpunpcklbw ymm2{k2}{z}, ymm6, ymm1
	vpunpcklbw zmm2{k7}{z}, zmm3, zword [0x4c6]
	vpunpcklbw xmm2, xmm4, oword [eax+1]
	vpunpcklbw xmm0, xmm7, oword [eax+64]
	vpunpcklbw ymm6, ymm7, yword [eax+1]
	vpunpcklbw ymm2, ymm1, yword [eax+64]
	vpunpcklbw xmm4, xmm1, oword [eax+1]
	vpunpcklbw xmm7, xmm4, oword [eax+64]
	vpunpcklbw ymm3, ymm5, yword [eax+1]
	vpunpcklbw ymm0, ymm5, yword [eax+64]
	vpunpcklbw zmm2, zmm3, zword [eax+1]
	vpunpcklbw zmm5, zmm2, zword [eax+64]
	vpunpcklbw xmm4, xmm5, [0x316]
	vpunpcklbw ymm0, ymm4, [0x570]
	vpunpcklbw xmm2, xmm4, [0x784]
	vpunpcklbw ymm3, ymm5, [0x50c]
	vpunpcklbw zmm1, zmm2, [0x784]
