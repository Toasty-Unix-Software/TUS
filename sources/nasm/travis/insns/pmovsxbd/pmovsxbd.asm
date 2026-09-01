default rel
	pmovsxbd xmm5, xmm7
	pmovsxbd xmm1, xmm5
	pmovsxbd xmm8, xmm13
	pmovsxbd xmm5, dword [eax+1]
	pmovsxbd xmm6, dword [eax+64]
	pmovsxbd xmm4, [0x34c]
