default rel
	pmuludq mm2, mm3
	pmuludq mm1, mm4
	pmuludq xmm2, xmm6
	pmuludq xmm1, xmm2
	pmuludq xmm11, xmm8
	pmuludq mm3, qword [eax+1]
	pmuludq mm4, qword [eax+64]
	pmuludq xmm5, oword [eax+1]
	pmuludq xmm6, oword [eax+64]
	pmuludq mm7, [0x166]
	pmuludq xmm5, [0x2a5]
