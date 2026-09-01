default rel
	vpshaw xmm5, oword [0xeeb], xmm6
	vpshaw xmm5, xmm6
	vpshaw xmm4, xmm3, xmm1
	vpshaw xmm2, xmm4, oword [0xe41]
	vpshaw xmm2, oword [0xe41]
	vpshaw xmm4, xmm6, xmm0
	vpshaw xmm10, xmm8, xmm15
	vpshaw xmm10, xmm11, xmm10
	vpshaw xmm6, oword [eax+1], xmm2
	vpshaw xmm7, oword [eax+64], xmm6
	vpshaw xmm2, xmm1, oword [eax+1]
	vpshaw xmm2, xmm1, oword [eax+64]
	vpshaw xmm6, [0xbba], xmm2
	vpshaw xmm4, xmm5, [0x8d5]
