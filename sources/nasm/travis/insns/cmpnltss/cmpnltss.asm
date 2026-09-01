default rel
	cmpnltss xmm1, xmm7
	cmpnltss xmm7, xmm6
	cmpnltss xmm11, xmm14
	cmpnltss xmm5, dword [eax+1]
	cmpnltss xmm0, dword [eax+64]
	cmpnltss xmm2, [0x2ee]
