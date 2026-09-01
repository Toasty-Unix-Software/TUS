default rel
	vfnmsub321ss xmm4, xmm4, xmm1
	vfnmsub321ss xmm2, xmm4, xmm3
	vfnmsub321ss xmm14, xmm12, xmm10
	vfnmsub321ss xmm3, xmm7, dword [eax+1]
	vfnmsub321ss xmm6, xmm7, dword [eax+64]
	vfnmsub321ss xmm5, xmm7, [0x34c]
