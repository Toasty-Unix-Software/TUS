default rel
	minss xmm3, xmm2
	minss xmm4, xmm2
	minss xmm12, xmm11
	minss xmm0, dword [eax+1]
	minss xmm3, dword [eax+64]
	minss xmm6, [0x178]
