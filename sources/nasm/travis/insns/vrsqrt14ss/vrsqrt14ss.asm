default rel
	vrsqrt14ss xmm3, xmm4, xmm3
	vrsqrt14ss xmm3, xmm3
	vrsqrt14ss xmm0, xmm4, dword [0x109]
	vrsqrt14ss xmm11, xmm12, xmm13
	vrsqrt14ss xmm30, xmm28, xmm20
	vrsqrt14ss xmm2{k7}, xmm5, xmm3
	vrsqrt14ss xmm1{k2}{z}, xmm2, xmm2
	vrsqrt14ss xmm1, xmm5, dword [eax+1]
	vrsqrt14ss xmm1, xmm3, dword [eax+64]
	vrsqrt14ss xmm4, xmm7, [0x2a5]
