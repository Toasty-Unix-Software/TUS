default rel
	pabsw mm5, mm7
	pabsw mm7, mm6
	pabsw xmm7, oword [0x186]
	pabsw xmm2, xmm2
	pabsw xmm13, xmm8
	pabsw mm3, qword [eax+1]
	pabsw mm7, qword [eax+64]
	pabsw xmm3, oword [eax+1]
	pabsw xmm7, oword [eax+64]
	pabsw mm4, [0x34e]
	pabsw xmm2, [0xa08]
