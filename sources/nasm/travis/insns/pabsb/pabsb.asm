default rel
	pabsb mm6, qword [0x4a2]
	pabsb mm6, qword [0x402]
	pabsb xmm0, xmm6
	pabsb xmm2, oword [0x4fb]
	pabsb xmm15, xmm15
	pabsb mm7, qword [eax+1]
	pabsb mm1, qword [eax+64]
	pabsb xmm4, oword [eax+1]
	pabsb xmm4, oword [eax+64]
	pabsb mm1, [0x7c8]
	pabsb xmm4, [0x291]
