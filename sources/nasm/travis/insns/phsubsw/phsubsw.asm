default rel
	phsubsw mm4, mm0
	phsubsw mm3, mm5
	phsubsw xmm2, oword [0x7a0]
	phsubsw xmm7, xmm7
	phsubsw xmm8, xmm8
	phsubsw mm4, qword [eax+1]
	phsubsw mm6, qword [eax+64]
	phsubsw xmm6, oword [eax+1]
	phsubsw xmm6, oword [eax+64]
	phsubsw mm3, [0xe3b]
	phsubsw xmm3, [0x36e]
