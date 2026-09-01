default rel
	cmppd xmm4, xmm3, 0xe7
	cmppd xmm6, xmm4, 0x11
	cmppd xmm14, xmm10, 0x7f
	cmppd xmm6, oword [eax+1], 0x2c
	cmppd xmm3, oword [eax+64], 0xf4
	cmppd xmm6, [0xaa8], 0xa1
