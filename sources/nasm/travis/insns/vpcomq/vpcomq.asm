default rel
	vpcomq xmm2, xmm5, xmm5, 0x4f
	vpcomq xmm2, xmm5, 0x4f
	vpcomq xmm0, xmm6, oword [0x3ca], 0x71
	vpcomq xmm11, xmm14, xmm14, 0xaf
	vpcomq xmm7, xmm1, oword [eax+1], 0x60
	vpcomq xmm3, xmm7, oword [eax+64], 0x59
	vpcomq xmm4, xmm3, [0x7c1], 0x1d
