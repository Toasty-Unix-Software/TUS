default rel
	vrndscaleps xmm7, xmm5, 0x34
	vrndscaleps xmm5, xmm6, 0xbd
	vrndscaleps ymm6, ymm2, 0xc3
	vrndscaleps ymm3, yword [0xe5e], 0x3a
	vrndscaleps zmm0, zword [0xe5c], 0x87
	vrndscaleps zmm2, zmm0, 0x5
	vrndscaleps xmm11, xmm9, 0xfb
	vrndscaleps ymm15, ymm9, 0x8c
	vrndscaleps zmm15, zmm10, 0x56
	vrndscaleps xmm24, xmm17, 0xea
	vrndscaleps ymm17, ymm19, 0xf1
	vrndscaleps zmm16, zmm31, 0xe2
	vrndscaleps xmm7{k2}, oword [0x13c], 0x3b
	vrndscaleps ymm2{k2}, ymm3, 0xa3
	vrndscaleps zmm1{k2}, zword [0xfe2], 0xc3
	vrndscaleps xmm1{k7}{z}, xmm2, 0x9f
	vrndscaleps ymm1{k2}{z}, ymm3, 0xef
	vrndscaleps zmm0{k4}{z}, zmm6, 0x8b
	vrndscaleps xmm5, oword [eax+1], 0x3b
	vrndscaleps xmm6, oword [eax+64], 0x6
	vrndscaleps ymm0, yword [eax+1], 0x95
	vrndscaleps ymm1, yword [eax+64], 0xd5
	vrndscaleps zmm3, zword [eax+1], 0xa
	vrndscaleps zmm5, zword [eax+64], 0xe9
	vrndscaleps xmm7, [0xd01], 0x6c
	vrndscaleps ymm3, [0x641], 0x27
	vrndscaleps zmm4, [0x2c8], 0x43
