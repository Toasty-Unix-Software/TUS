default rel
	vrsqrtss xmm1, xmm4, dword [0xeda]
	vrsqrtss xmm1, dword [0xeda]
	vrsqrtss xmm3, xmm1, xmm5
	vrsqrtss xmm13, xmm8, xmm15
	vrsqrtss xmm0, xmm1, dword [eax+1]
	vrsqrtss xmm6, xmm3, dword [eax+64]
	vrsqrtss xmm7, xmm4, [0xece]
