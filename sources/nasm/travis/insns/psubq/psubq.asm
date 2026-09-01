default rel
	psubq mm4, mm3
	psubq mm4, mm3
	psubq xmm0, xmm4
	psubq xmm4, xmm4
	psubq xmm11, xmm11
	psubq mm7, qword [eax+1]
	psubq mm7, qword [eax+64]
	psubq xmm3, oword [eax+1]
	psubq xmm6, oword [eax+64]
	psubq mm0, [0x90b]
	psubq xmm3, [0x8b8]
