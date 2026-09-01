default rel
	pminsw mm0, mm0
	pminsw mm0, mm3
	pminsw xmm7, xmm0
	pminsw xmm6, oword [0x7fa]
	pminsw xmm8, xmm15
	pminsw mm7, qword [eax+1]
	pminsw mm2, qword [eax+64]
	pminsw xmm5, oword [eax+1]
	pminsw xmm3, oword [eax+64]
	pminsw mm7, [0xeef]
	pminsw xmm1, [0x67b]
