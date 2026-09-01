default rel
	pmaxsd xmm0, xmm4
	pmaxsd xmm4, oword [0x15b]
	pmaxsd xmm13, xmm14
	pmaxsd xmm4, oword [eax+1]
	pmaxsd xmm3, oword [eax+64]
	pmaxsd xmm5, [0xba2]
