default rel
	phsubw mm7, mm5
	phsubw mm7, mm7
	phsubw xmm0, xmm2
	phsubw xmm6, oword [0xdce]
	phsubw xmm9, xmm8
	phsubw mm4, qword [eax+1]
	phsubw mm7, qword [eax+64]
	phsubw xmm6, oword [eax+1]
	phsubw xmm4, oword [eax+64]
	phsubw mm0, [0x486]
	phsubw xmm4, [0x219]
