default rel
	vpcomw xmm4, xmm2, xmm5, 0x4f
	vpcomw xmm4, xmm5, 0x4f
	vpcomw xmm1, xmm4, xmm2, 0xb7
	vpcomw xmm15, xmm9, xmm15, 0xac
	vpcomw xmm3, xmm3, oword [eax+1], 0x9f
	vpcomw xmm4, xmm1, oword [eax+64], 0x3a
	vpcomw xmm7, xmm6, [0xe39], 0xb4
