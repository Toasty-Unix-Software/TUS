default rel
	psrlq mm4, mm3
	psrlq mm0, mm4
	psrlq mm4, 0x3d
	psrlq mm3, 0x36
	psrlq xmm4, oword [0xf70]
	psrlq xmm3, oword [0x8e7]
	psrlq xmm7, 0x71
	psrlq xmm6, 0x37
	psrlq xmm11, xmm15
	psrlq xmm13, 0x6a
	psrlq mm4, qword [eax+1]
	psrlq mm5, qword [eax+64]
	psrlq xmm0, oword [eax+1]
	psrlq xmm2, oword [eax+64]
	psrlq mm4, [0x48f]
	psrlq xmm6, [0x33b]
