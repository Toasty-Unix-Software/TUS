default rel
	shufps xmm4, xmm2, 0x3e
	shufps xmm5, xmm7, 0x64
	shufps xmm14, xmm13, 0x4f
	shufps xmm7, oword [eax+1], 0x40
	shufps xmm5, oword [eax+64], 0x3e
	shufps xmm0, [0x15d], 0xb
