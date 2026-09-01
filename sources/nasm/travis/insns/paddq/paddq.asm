default rel
	paddq mm7, mm2
	paddq mm1, qword [0x86a]
	paddq xmm7, xmm1
	paddq xmm2, xmm3
	paddq xmm13, xmm10
	paddq mm7, qword [eax+1]
	paddq mm4, qword [eax+64]
	paddq xmm5, oword [eax+1]
	paddq xmm7, oword [eax+64]
	paddq mm2, [0xd9f]
	paddq xmm1, [0x1ed]
