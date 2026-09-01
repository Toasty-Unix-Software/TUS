default rel
	vgetmantsh xmm5, xmm4, xmm7, 0x22
	vgetmantsh xmm3, xmm7, xmm4, 0xa5
	vgetmantsh xmm13, xmm15, xmm9, 0x5f
	vgetmantsh xmm30, xmm29, xmm28, 0x46
	vgetmantsh xmm5{k4}, xmm1, word [0xeb1], 0x1b
	vgetmantsh xmm6{k5}{z}, xmm7, xmm6, 0x87
	vgetmantsh xmm4, xmm5, word [eax+1], 0x88
	vgetmantsh xmm2, xmm5, word [eax+64], 0x62
	vgetmantsh xmm7, xmm5, [0xecd], 0xa0
