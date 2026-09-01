default rel
	punpckldq mm2, mm1
	punpckldq mm1, qword [0x7dd]
	punpckldq xmm4, oword [0xbbc]
	punpckldq xmm4, xmm5
	punpckldq xmm13, xmm11
	punpckldq mm2, qword [eax+1]
	punpckldq mm6, qword [eax+64]
	punpckldq xmm5, oword [eax+1]
	punpckldq xmm5, oword [eax+64]
	punpckldq mm2, [0x7b2]
	punpckldq xmm0, [0x75d]
