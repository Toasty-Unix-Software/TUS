default rel
	aesdeclast xmm6, xmm7
	aesdeclast xmm6, oword [0x5f8]
	aesdeclast xmm13, xmm9
	aesdeclast xmm0, oword [eax+1]
	aesdeclast xmm6, oword [eax+64]
	aesdeclast xmm2, [0x2d0]
