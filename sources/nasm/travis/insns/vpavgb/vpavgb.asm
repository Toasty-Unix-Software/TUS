default rel
	vpavgb xmm7, xmm2, xmm6
	vpavgb xmm7, xmm6
	vpavgb xmm2, xmm6, xmm6
	vpavgb ymm4, ymm3, ymm7
	vpavgb ymm4, ymm7
	vpavgb ymm2, ymm5, ymm2
	vpavgb xmm4, xmm4, xmm1
	vpavgb xmm4, xmm1
	vpavgb xmm6, xmm5, xmm2
	vpavgb ymm1, ymm4, ymm1
	vpavgb ymm1, ymm1
	vpavgb ymm2, ymm1, ymm0
	vpavgb xmm9, xmm8, xmm14
	vpavgb ymm10, ymm14, ymm14
	vpavgb xmm8, xmm15, xmm10
	vpavgb ymm9, ymm10, ymm15
	vpavgb xmm30, xmm26, xmm17
	vpavgb ymm22, ymm17, ymm25
	vpavgb xmm23, xmm18, xmm26
	vpavgb ymm27, ymm19, ymm25
	vpavgb xmm1{k7}, xmm4, oword [0x51d]
	vpavgb ymm5{k2}, ymm2, yword [0xe04]
	vpavgb zmm4{k2}, zmm7, zword [0x9ce]
	vpavgb xmm1{k6}{z}, xmm0, xmm5
	vpavgb ymm4{k5}{z}, ymm7, yword [0x758]
	vpavgb zmm0{k7}{z}, zmm1, zmm4
	vpavgb xmm3, xmm6, oword [eax+1]
	vpavgb xmm7, xmm6, oword [eax+64]
	vpavgb ymm3, ymm6, yword [eax+1]
	vpavgb ymm5, ymm5, yword [eax+64]
	vpavgb xmm4, xmm0, oword [eax+1]
	vpavgb xmm0, xmm6, oword [eax+64]
	vpavgb ymm2, ymm3, yword [eax+1]
	vpavgb ymm6, ymm3, yword [eax+64]
	vpavgb zmm4, zmm7, zword [eax+1]
	vpavgb zmm0, zmm3, zword [eax+64]
	vpavgb xmm3, xmm5, [0x14b]
	vpavgb ymm7, ymm6, [0x4d5]
	vpavgb xmm7, xmm0, [0xec0]
	vpavgb ymm2, ymm4, [0xe87]
	vpavgb zmm1, zmm1, [0xa0f]
