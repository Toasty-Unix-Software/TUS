default rel
	pcmpgtb mm4, mm7
	pcmpgtb mm7, mm2
	pcmpgtb xmm6, xmm3
	pcmpgtb xmm5, oword [0xe9b]
	pcmpgtb xmm8, xmm10
	pcmpgtb mm3, qword [eax+1]
	pcmpgtb mm1, qword [eax+64]
	pcmpgtb xmm5, oword [eax+1]
	pcmpgtb xmm1, oword [eax+64]
	pcmpgtb mm0, [0xfb4]
	pcmpgtb xmm6, [0x102]
