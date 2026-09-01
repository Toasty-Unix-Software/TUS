default rel
	psubw mm2, mm1
	psubw mm1, qword [0x645]
	psubw xmm6, xmm2
	psubw xmm6, xmm7
	psubw xmm12, xmm11
	psubw mm4, qword [eax+1]
	psubw mm4, qword [eax+64]
	psubw xmm5, oword [eax+1]
	psubw xmm0, oword [eax+64]
	psubw mm2, [0x129]
	psubw xmm4, [0x994]
