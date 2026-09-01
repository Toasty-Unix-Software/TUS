default rel
	cmpneqss xmm7, xmm0
	cmpneqss xmm5, dword [0xcae]
	cmpneqss xmm9, xmm14
	cmpneqss xmm1, dword [eax+1]
	cmpneqss xmm4, dword [eax+64]
	cmpneqss xmm6, [0xd00]
