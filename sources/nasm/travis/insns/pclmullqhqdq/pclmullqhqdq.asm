default rel
	pclmullqhqdq xmm1, xmm5
	pclmullqhqdq xmm6, oword [0xe44]
	pclmullqhqdq xmm12, xmm14
	pclmullqhqdq xmm2, oword [eax+1]
	pclmullqhqdq xmm6, oword [eax+64]
	pclmullqhqdq xmm0, [0x5cd]
