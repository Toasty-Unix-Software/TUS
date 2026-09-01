default rel
	paddusw mm7, mm6
	paddusw mm2, qword [0x41f]
	paddusw xmm3, xmm0
	paddusw xmm3, oword [0x2cf]
	paddusw xmm15, xmm15
	paddusw mm5, qword [eax+1]
	paddusw mm4, qword [eax+64]
	paddusw xmm3, oword [eax+1]
	paddusw xmm2, oword [eax+64]
	paddusw mm3, [0x5fa]
	paddusw xmm1, [0xa8e]
