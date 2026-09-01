default rel
	addss xmm2, xmm3
	addss xmm4, xmm2
	addss xmm15, xmm12
	addss xmm5, dword [eax+1]
	addss xmm5, dword [eax+64]
	addss xmm1, [0xded]
