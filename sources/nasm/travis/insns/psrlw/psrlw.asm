default rel
	psrlw mm1, mm4
	psrlw mm1, mm2
	psrlw mm1, 0x79
	psrlw mm3, 0x40
	psrlw xmm6, xmm1
	psrlw xmm7, xmm4
	psrlw xmm6, 0x35
	psrlw xmm3, 0x55
	psrlw xmm13, xmm14
	psrlw xmm15, 0xd
	psrlw mm6, qword [eax+1]
	psrlw mm1, qword [eax+64]
	psrlw xmm5, oword [eax+1]
	psrlw xmm4, oword [eax+64]
	psrlw mm4, [0x8f6]
	psrlw xmm1, [0xae9]
