default rel
	vrndscaleph xmm2, oword [0x2b2], 0x74
	vrndscaleph xmm6, xmm1, 0x1f
	vrndscaleph ymm4, yword [0x937], 0xb8
	vrndscaleph ymm3, ymm1, 0xe6
	vrndscaleph zmm1, zmm0, 0x56
	vrndscaleph zmm4, zword [0x53c], 0x8
	vrndscaleph xmm10, xmm11, 0x48
	vrndscaleph ymm12, ymm11, 0x91
	vrndscaleph zmm13, zmm10, 0x97
	vrndscaleph xmm22, xmm29, 0x9a
	vrndscaleph ymm27, ymm27, 0x11
	vrndscaleph zmm16, zmm27, 0xd0
	vrndscaleph xmm1{k5}, xmm1, 0x52
	vrndscaleph ymm4{k4}, ymm0, 0xf8
	vrndscaleph zmm6{k1}, zmm0, 0xd7
	vrndscaleph xmm4{k4}{z}, xmm6, 0x59
	vrndscaleph ymm0{k1}{z}, yword [0x8e9], 0xc3
	vrndscaleph zmm1{k2}{z}, zmm4, 0x45
	vrndscaleph xmm4, oword [eax+1], 0x3e
	vrndscaleph xmm6, oword [eax+64], 0x6f
	vrndscaleph ymm4, yword [eax+1], 0xea
	vrndscaleph ymm6, yword [eax+64], 0x2a
	vrndscaleph zmm4, zword [eax+1], 0xdf
	vrndscaleph zmm5, zword [eax+64], 0xea
	vrndscaleph xmm7, [0x2de], 0x49
	vrndscaleph ymm1, [0x8da], 0xa9
	vrndscaleph zmm3, [0x66f], 0x95
