default rel
	andps xmm7, xmm4
	andps xmm7, xmm6
	andps xmm14, xmm11
	andps xmm5, oword [eax+1]
	andps xmm6, oword [eax+64]
	andps xmm5, [0xf61]
