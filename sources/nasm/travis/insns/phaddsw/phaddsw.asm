default rel
	phaddsw mm5, mm4
	phaddsw mm4, mm1
	phaddsw xmm3, xmm3
	phaddsw xmm5, oword [0xc90]
	phaddsw xmm8, xmm12
	phaddsw mm1, qword [eax+1]
	phaddsw mm5, qword [eax+64]
	phaddsw xmm2, oword [eax+1]
	phaddsw xmm0, oword [eax+64]
	phaddsw mm7, [0x85c]
	phaddsw xmm4, [0x496]
