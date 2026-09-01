default rel
	xorps xmm3, oword [0xc38]
	xorps xmm2, oword [0xd59]
	xorps xmm9, xmm9
	xorps xmm6, oword [eax+1]
	xorps xmm6, oword [eax+64]
	xorps xmm7, [0x883]
