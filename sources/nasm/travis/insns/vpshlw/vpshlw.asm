default rel
	vpshlw xmm4, xmm4, xmm2
	vpshlw xmm4, xmm2
	vpshlw xmm5, oword [0xc4f], xmm0
	vpshlw xmm2, xmm5, xmm1
	vpshlw xmm2, xmm1
	vpshlw xmm7, xmm4, xmm6
	vpshlw xmm10, xmm9, xmm12
	vpshlw xmm13, xmm9, xmm15
	vpshlw xmm5, oword [eax+1], xmm4
	vpshlw xmm5, oword [eax+64], xmm4
	vpshlw xmm3, xmm3, oword [eax+1]
	vpshlw xmm0, xmm4, oword [eax+64]
	vpshlw xmm3, [0x67d], xmm2
	vpshlw xmm7, xmm5, [0x9e5]
