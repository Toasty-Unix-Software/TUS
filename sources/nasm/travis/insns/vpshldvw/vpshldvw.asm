default rel
	vpshldvw xmm6, xmm6, oword [0xc56]
	vpshldvw xmm6, oword [0xc56]
	vpshldvw xmm5, xmm6, oword [0xd83]
	vpshldvw ymm0, ymm2, ymm7
	vpshldvw ymm0, ymm7
	vpshldvw ymm1, ymm6, yword [0xae8]
	vpshldvw zmm2, zmm0, zword [0x186]
	vpshldvw zmm2, zword [0x186]
	vpshldvw zmm0, zmm7, zword [0x590]
	vpshldvw xmm11, xmm14, xmm14
	vpshldvw ymm14, ymm13, ymm9
	vpshldvw zmm8, zmm15, zmm14
	vpshldvw xmm17, xmm25, xmm29
	vpshldvw ymm18, ymm24, ymm27
	vpshldvw zmm31, zmm25, zmm31
	vpshldvw xmm7{k7}, xmm0, oword [0x1e4]
	vpshldvw ymm7{k5}, ymm5, yword [0x372]
	vpshldvw zmm1{k3}, zmm6, zmm6
	vpshldvw xmm6{k1}{z}, xmm5, oword [0x596]
	vpshldvw ymm7{k5}{z}, ymm0, ymm7
	vpshldvw zmm3{k2}{z}, zmm7, zmm7
	vpshldvw xmm4, xmm2, oword [eax+1]
	vpshldvw xmm6, xmm1, oword [eax+64]
	vpshldvw ymm2, ymm2, yword [eax+1]
	vpshldvw ymm0, ymm3, yword [eax+64]
	vpshldvw zmm6, zmm7, zword [eax+1]
	vpshldvw zmm0, zmm0, zword [eax+64]
	vpshldvw xmm2, xmm4, [0x74a]
	vpshldvw ymm5, ymm3, [0x7cd]
	vpshldvw zmm6, zmm4, [0xb88]
