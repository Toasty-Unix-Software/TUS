default rel
	pcmpeqb mm2, mm0
	pcmpeqb mm6, mm4
	pcmpeqb xmm0, oword [0x459]
	pcmpeqb xmm0, xmm0
	pcmpeqb xmm8, xmm12
	pcmpeqb mm4, qword [eax+1]
	pcmpeqb mm2, qword [eax+64]
	pcmpeqb xmm3, oword [eax+1]
	pcmpeqb xmm5, oword [eax+64]
	pcmpeqb mm2, [0x799]
	pcmpeqb xmm6, [0xf9e]
