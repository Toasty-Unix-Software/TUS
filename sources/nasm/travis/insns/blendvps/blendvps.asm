default rel
	blendvps xmm1, xmm6, xmm0
	blendvps xmm7, xmm6, xmm0
	blendvps xmm1, xmm1
	blendvps xmm6, xmm0
	blendvps xmm12, xmm10, xmm0
	blendvps xmm14, xmm15
	blendvps xmm3, oword [eax+1], xmm0
	blendvps xmm7, oword [eax+64], xmm0
	blendvps xmm5, oword [eax+1]
	blendvps xmm5, oword [eax+64]
	blendvps xmm1, [0x95d], xmm0
	blendvps xmm5, [0x4ad]
