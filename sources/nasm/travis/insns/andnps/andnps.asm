default rel
	andnps xmm3, xmm7
	andnps xmm3, oword [0x590]
	andnps xmm11, xmm11
	andnps xmm6, oword [eax+1]
	andnps xmm3, oword [eax+64]
	andnps xmm0, [0x6d4]
