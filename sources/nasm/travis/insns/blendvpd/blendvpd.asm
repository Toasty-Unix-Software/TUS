default rel
	blendvpd xmm3, xmm3, xmm0
	blendvpd xmm7, oword [0x2fe], xmm0
	blendvpd xmm3, oword [0xab1]
	blendvpd xmm5, oword [0xcd6]
	blendvpd xmm12, xmm8, xmm0
	blendvpd xmm8, xmm8
	blendvpd xmm3, oword [eax+1], xmm0
	blendvpd xmm1, oword [eax+64], xmm0
	blendvpd xmm5, oword [eax+1]
	blendvpd xmm4, oword [eax+64]
	blendvpd xmm2, [0x98d], xmm0
	blendvpd xmm6, [0x683]
