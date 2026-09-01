default rel
	vpcomub xmm5, xmm6, oword [0xd15], 0xc4
	vpcomub xmm5, oword [0xd15], 0xc4
	vpcomub xmm4, xmm3, xmm5, 0x78
	vpcomub xmm12, xmm14, xmm14, 0xc7
	vpcomub xmm3, xmm1, oword [eax+1], 0x46
	vpcomub xmm6, xmm1, oword [eax+64], 0xeb
	vpcomub xmm7, xmm1, [0x2f7], 0xe0
