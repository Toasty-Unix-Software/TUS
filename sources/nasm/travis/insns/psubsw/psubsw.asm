default rel
	psubsw mm0, mm7
	psubsw mm0, mm3
	psubsw xmm2, oword [0xd52]
	psubsw xmm4, oword [0x8c2]
	psubsw xmm9, xmm12
	psubsw mm5, qword [eax+1]
	psubsw mm2, qword [eax+64]
	psubsw xmm2, oword [eax+1]
	psubsw xmm4, oword [eax+64]
	psubsw mm4, [0x378]
	psubsw xmm1, [0xcd0]
