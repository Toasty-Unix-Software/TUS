default rel
	vpabsb xmm1, xmm0
	vpabsb xmm3, oword [0x4a8]
	vpabsb ymm0, ymm1
	vpabsb ymm3, ymm5
	vpabsb xmm7, xmm2
	vpabsb xmm2, xmm7
	vpabsb ymm7, ymm4
	vpabsb ymm2, ymm5
	vpabsb xmm14, xmm15
	vpabsb ymm9, ymm14
	vpabsb xmm9, xmm9
	vpabsb ymm13, ymm8
	vpabsb xmm26, xmm17
	vpabsb ymm18, ymm25
	vpabsb xmm20, xmm19
	vpabsb ymm20, ymm19
	vpabsb xmm6{k4}, xmm4
	vpabsb ymm0{k7}, ymm2
	vpabsb zmm6{k2}, zmm5
	vpabsb xmm1{k7}{z}, oword [0x8c6]
	vpabsb ymm7{k6}{z}, ymm0
	vpabsb zmm2{k4}{z}, zword [0x86e]
	vpabsb xmm7, oword [eax+1]
	vpabsb xmm3, oword [eax+64]
	vpabsb ymm3, yword [eax+1]
	vpabsb ymm2, yword [eax+64]
	vpabsb xmm1, oword [eax+1]
	vpabsb xmm1, oword [eax+64]
	vpabsb ymm6, yword [eax+1]
	vpabsb ymm5, yword [eax+64]
	vpabsb zmm6, zword [eax+1]
	vpabsb zmm5, zword [eax+64]
	vpabsb xmm2, [0x9bb]
	vpabsb ymm4, [0x472]
	vpabsb xmm0, [0x1eb]
	vpabsb ymm0, [0x271]
	vpabsb zmm7, [0xb6a]
