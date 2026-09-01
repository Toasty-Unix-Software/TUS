default rel
	roundpd xmm1, xmm7, 0x7c
	roundpd xmm1, xmm6, 0x6e
	roundpd xmm14, xmm11, 0x1e
	roundpd xmm6, oword [eax+1], 0xc9
	roundpd xmm3, oword [eax+64], 0x4
	roundpd xmm7, [0xe8c], 0x2a
