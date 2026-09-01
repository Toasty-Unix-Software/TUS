default rel
	vpblendmw xmm0, xmm6, xmm0
	vpblendmw xmm1, xmm0, xmm7
	vpblendmw ymm3, ymm2, yword [0xe21]
	vpblendmw ymm6, ymm6, yword [0x257]
	vpblendmw zmm1, zmm7, zword [0xa16]
	vpblendmw zmm7, zmm5, zmm1
	vpblendmw xmm10, xmm8, xmm15
	vpblendmw ymm13, ymm15, ymm12
	vpblendmw zmm9, zmm14, zmm12
	vpblendmw xmm31, xmm24, xmm21
	vpblendmw ymm24, ymm16, ymm26
	vpblendmw zmm20, zmm30, zmm22
	vpblendmw xmm5{k7}, xmm5, xmm1
	vpblendmw ymm1{k5}, ymm3, ymm0
	vpblendmw zmm5{k7}, zmm2, zword [0xe9a]
	vpblendmw xmm6{k7}{z}, xmm4, xmm7
	vpblendmw ymm7{k2}{z}, ymm5, ymm2
	vpblendmw zmm1{k2}{z}, zmm4, zmm7
	vpblendmw xmm6, xmm3, oword [eax+1]
	vpblendmw xmm3, xmm5, oword [eax+64]
	vpblendmw ymm1, ymm7, yword [eax+1]
	vpblendmw ymm4, ymm5, yword [eax+64]
	vpblendmw zmm5, zmm6, zword [eax+1]
	vpblendmw zmm4, zmm0, zword [eax+64]
	vpblendmw xmm5, xmm4, [0x4cb]
	vpblendmw ymm5, ymm2, [0xdf9]
	vpblendmw zmm5, zmm1, [0x1a0]
