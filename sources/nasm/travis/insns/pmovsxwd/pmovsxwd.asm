default rel
	pmovsxwd xmm6, qword [0xdcb]
	pmovsxwd xmm4, xmm7
	pmovsxwd xmm10, xmm13
	pmovsxwd xmm3, qword [eax+1]
	pmovsxwd xmm0, qword [eax+64]
	pmovsxwd xmm1, [0x3f3]
