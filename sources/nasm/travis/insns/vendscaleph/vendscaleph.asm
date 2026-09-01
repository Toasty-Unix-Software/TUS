default rel
	vendscaleph xmm4, xmm7, 0xe1
	vendscaleph xmm7, xmm7, 0x53
	vendscaleph ymm3, ymm5, 0x15
	vendscaleph ymm5, ymm2, 0x80
	vendscaleph zmm7, zmm4, 0xe
	vendscaleph zmm4, zmm5, 0x2c
	vendscaleph xmm10, xmm13, 0x69
	vendscaleph ymm9, ymm11, 0x61
	vendscaleph zmm8, zmm10, 0xca
	vendscaleph xmm30, xmm21, 0xd4
	vendscaleph ymm31, ymm30, 0x69
	vendscaleph zmm20, zmm22, 0x2
	vendscaleph xmm2{k6}, xmm5, 0x63
	vendscaleph ymm5{k4}, yword [0xa01], 0xf1
	vendscaleph zmm5{k2}, zword [0xc84], 0x51
	vendscaleph xmm6{k3}{z}, xmm3, 0xc7
	vendscaleph ymm7{k3}{z}, ymm0, 0xc6
	vendscaleph zmm0{k4}{z}, zword [0x34a], 0x4b
	vendscaleph xmm3, oword [eax+1], 0x8d
	vendscaleph xmm1, oword [eax+64], 0x56
	vendscaleph ymm3, yword [eax+1], 0x68
	vendscaleph ymm0, yword [eax+64], 0x5f
	vendscaleph zmm4, zword [eax+1], 0xac
	vendscaleph zmm1, zword [eax+64], 0x77
	vendscaleph xmm5, [0xfd7], 0x86
	vendscaleph ymm4, [0xd74], 0xaf
	vendscaleph zmm5, [0x5d3], 0xf5
