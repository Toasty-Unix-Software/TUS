default rel
	minsd xmm6, oword [0x1df]
	minsd xmm3, xmm7
	minsd xmm13, xmm9
	minsd xmm7, oword [eax+1]
	minsd xmm4, oword [eax+64]
	minsd xmm5, [0xfc9]
