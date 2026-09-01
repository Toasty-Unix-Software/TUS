default rel
	vreducesh xmm2, xmm4, xmm6, 0xc0
	vreducesh xmm2, xmm6, 0xc0
	vreducesh xmm3, xmm4, xmm0, 0xaf
	vreducesh xmm8, xmm9, xmm14, 0x68
	vreducesh xmm23, xmm25, xmm22, 0x52
	vreducesh xmm6{k7}, xmm0, xmm2, 0xf8
	vreducesh xmm6{k3}{z}, xmm5, xmm6, 0x98
	vreducesh xmm1, xmm1, word [eax+1], 0xb4
	vreducesh xmm1, xmm6, word [eax+64], 0xd1
	vreducesh xmm0, xmm2, [0xe93], 0x7b
