default rel
	movapd xmm2, oword [0x731]
	movapd xmm7, xmm4
	movapd xmm0, xmm3
	movapd xmm2, xmm7
	movapd xmm11, xmm13
	movapd xmm13, xmm13
	movapd xmm1, oword [eax+1]
	movapd xmm5, oword [eax+64]
	movapd oword [eax+1], xmm0
	movapd oword [eax+64], xmm4
	movapd xmm0, [0x73f]
	movapd [0x332], xmm6
