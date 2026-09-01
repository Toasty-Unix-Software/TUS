default rel
	punpckhqdq xmm3, xmm2
	punpckhqdq xmm0, oword [0xfee]
	punpckhqdq xmm8, xmm13
	punpckhqdq xmm7, oword [eax+1]
	punpckhqdq xmm1, oword [eax+64]
	punpckhqdq xmm2, [0x183]
