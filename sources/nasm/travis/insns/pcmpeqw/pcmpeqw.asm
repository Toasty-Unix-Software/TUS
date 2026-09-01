default rel
	pcmpeqw mm2, mm1
	pcmpeqw mm4, mm0
	pcmpeqw xmm3, oword [0x972]
	pcmpeqw xmm1, xmm2
	pcmpeqw xmm13, xmm11
	pcmpeqw mm2, qword [eax+1]
	pcmpeqw mm6, qword [eax+64]
	pcmpeqw xmm3, oword [eax+1]
	pcmpeqw xmm5, oword [eax+64]
	pcmpeqw mm3, [0x90d]
	pcmpeqw xmm7, [0x5e5]
