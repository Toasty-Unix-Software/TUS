default rel
	pmovsxbq xmm3, xmm7
	pmovsxbq xmm1, xmm0
	pmovsxbq xmm13, xmm11
	pmovsxbq xmm3, word [eax+1]
	pmovsxbq xmm5, word [eax+64]
	pmovsxbq xmm0, [0xf1a]
