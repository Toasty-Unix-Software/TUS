default rel
	pmovsxwq xmm5, xmm4
	pmovsxwq xmm7, xmm2
	pmovsxwq xmm15, xmm13
	pmovsxwq xmm2, dword [eax+1]
	pmovsxwq xmm2, dword [eax+64]
	pmovsxwq xmm6, [0x2ea]
