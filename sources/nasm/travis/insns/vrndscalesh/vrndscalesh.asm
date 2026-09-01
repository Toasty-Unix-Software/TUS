default rel
	vrndscalesh xmm0, xmm7, xmm4, 0xa4
	vrndscalesh xmm0, xmm4, 0xa4
	vrndscalesh xmm3, xmm5, word [0x196], 0x6a
	vrndscalesh xmm13, xmm8, xmm13, 0x6a
	vrndscalesh xmm22, xmm25, xmm31, 0x6c
	vrndscalesh xmm4{k6}, xmm5, xmm1, 0x4b
	vrndscalesh xmm5{k1}{z}, xmm1, word [0x71f], 0x73
	vrndscalesh xmm6, xmm2, word [eax+1], 0xd1
	vrndscalesh xmm2, xmm6, word [eax+64], 0x4c
	vrndscalesh xmm6, xmm3, [0xd14], 0xd3
