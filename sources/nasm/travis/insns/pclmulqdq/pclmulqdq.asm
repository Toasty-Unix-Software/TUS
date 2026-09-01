default rel
	pclmulqdq xmm0, xmm5, 0x6
	pclmulqdq xmm6, xmm7, 0xed
	pclmulqdq xmm9, xmm14, 0x37
	pclmulqdq xmm7, oword [eax+1], 0xbf
	pclmulqdq xmm1, oword [eax+64], 0xac
	pclmulqdq xmm3, [0x27e], 0x44
