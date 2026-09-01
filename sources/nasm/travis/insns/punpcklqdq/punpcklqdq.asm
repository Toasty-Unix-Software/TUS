default rel
	punpcklqdq xmm6, xmm1
	punpcklqdq xmm4, oword [0x674]
	punpcklqdq xmm15, xmm14
	punpcklqdq xmm0, oword [eax+1]
	punpcklqdq xmm7, oword [eax+64]
	punpcklqdq xmm2, [0x4ba]
