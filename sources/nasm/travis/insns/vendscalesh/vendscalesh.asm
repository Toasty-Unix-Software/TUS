default rel
	vendscalesh xmm4, xmm5, xmm4, 0xef
	vendscalesh xmm4, xmm4, 0xef
	vendscalesh xmm3, xmm0, xmm2, 0x74
	vendscalesh xmm10, xmm11, xmm15, 0x6a
	vendscalesh xmm19, xmm31, xmm25, 0x19
	vendscalesh xmm4{k2}, xmm2, xmm0, 0xc9
	vendscalesh xmm4{k5}{z}, xmm1, xmm1, 0x9
	vendscalesh xmm7, xmm1, word [eax+1], 0x22
	vendscalesh xmm3, xmm1, word [eax+64], 0x44
	vendscalesh xmm6, xmm2, [0x780], 0x89
