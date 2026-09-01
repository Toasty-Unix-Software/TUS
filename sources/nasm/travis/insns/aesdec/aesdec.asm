default rel
	aesdec xmm0, oword [0x639]
	aesdec xmm4, xmm5
	aesdec xmm14, xmm8
	aesdec xmm6, oword [eax+1]
	aesdec xmm7, oword [eax+64]
	aesdec xmm7, [0x808]
