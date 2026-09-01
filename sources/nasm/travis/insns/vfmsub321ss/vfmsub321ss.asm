default rel
	vfmsub321ss xmm6, xmm4, xmm4
	vfmsub321ss xmm0, xmm2, xmm0
	vfmsub321ss xmm8, xmm13, xmm12
	vfmsub321ss xmm6, xmm2, dword [eax+1]
	vfmsub321ss xmm1, xmm3, dword [eax+64]
	vfmsub321ss xmm1, xmm0, [0xc73]
