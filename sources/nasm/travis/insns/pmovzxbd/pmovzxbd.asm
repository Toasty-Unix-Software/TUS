default rel
	pmovzxbd xmm6, xmm5
	pmovzxbd xmm2, xmm1
	pmovzxbd xmm15, xmm14
	pmovzxbd xmm1, dword [eax+1]
	pmovzxbd xmm6, dword [eax+64]
	pmovzxbd xmm4, [0x7b6]
