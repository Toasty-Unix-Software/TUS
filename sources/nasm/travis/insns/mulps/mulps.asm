default rel
	mulps xmm0, xmm6
	mulps xmm6, oword [0x387]
	mulps xmm8, xmm11
	mulps xmm4, oword [eax+1]
	mulps xmm5, oword [eax+64]
	mulps xmm1, [0x85f]
