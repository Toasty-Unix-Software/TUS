default rel
	pcmpeqq xmm7, xmm6
	pcmpeqq xmm0, oword [0x7a2]
	pcmpeqq xmm10, xmm15
	pcmpeqq xmm5, oword [eax+1]
	pcmpeqq xmm1, oword [eax+64]
	pcmpeqq xmm6, [0x6f4]
