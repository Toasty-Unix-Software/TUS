default rel
	vminmaxsh xmm2, xmm2, xmm6, 0x65
	vminmaxsh xmm4, xmm6, xmm1, 0x84
	vminmaxsh xmm12, xmm10, xmm14, 0xba
	vminmaxsh xmm22, xmm28, xmm23, 0xca
	vminmaxsh xmm2{k4}, xmm7, word [0x41c], 0x4
	vminmaxsh xmm3{k1}{z}, xmm0, word [0xdb9], 0x62
	vminmaxsh xmm6, xmm4, word [eax+1], 0xb5
	vminmaxsh xmm7, xmm2, word [eax+64], 0x5
	vminmaxsh xmm0, xmm3, [0x7e1], 0xed
