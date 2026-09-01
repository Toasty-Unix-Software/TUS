default rel
	pclmullqlqdq xmm6, oword [0xb6a]
	pclmullqlqdq xmm3, oword [0x694]
	pclmullqlqdq xmm8, xmm15
	pclmullqlqdq xmm3, oword [eax+1]
	pclmullqlqdq xmm4, oword [eax+64]
	pclmullqlqdq xmm1, [0x63a]
