default rel
	aeskeygenassist xmm4, xmm0, 0x14
	aeskeygenassist xmm5, xmm3, 0xfd
	aeskeygenassist xmm14, xmm11, 0x2d
	aeskeygenassist xmm7, oword [eax+1], 0xe
	aeskeygenassist xmm7, oword [eax+64], 0xf2
	aeskeygenassist xmm1, [0x59c], 0x28
