default rel
	pcmpgtq xmm7, xmm0
	pcmpgtq xmm2, xmm5
	pcmpgtq xmm10, xmm11
	pcmpgtq xmm0, oword [eax+1]
	pcmpgtq xmm2, oword [eax+64]
	pcmpgtq xmm0, [0x43c]
