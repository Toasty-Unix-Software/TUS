default rel
	pmaddwd mm1, mm5
	pmaddwd mm1, qword [0xdfb]
	pmaddwd xmm5, oword [0x9bf]
	pmaddwd xmm0, xmm1
	pmaddwd xmm14, xmm12
	pmaddwd mm2, qword [eax+1]
	pmaddwd mm5, qword [eax+64]
	pmaddwd xmm0, oword [eax+1]
	pmaddwd xmm4, oword [eax+64]
	pmaddwd mm7, [0x984]
	pmaddwd xmm7, [0x5bf]
