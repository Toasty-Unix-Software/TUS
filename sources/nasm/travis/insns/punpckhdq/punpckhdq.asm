default rel
	punpckhdq mm0, mm5
	punpckhdq mm7, qword [0x876]
	punpckhdq xmm2, xmm0
	punpckhdq xmm6, xmm1
	punpckhdq xmm11, xmm14
	punpckhdq mm1, qword [eax+1]
	punpckhdq mm0, qword [eax+64]
	punpckhdq xmm1, oword [eax+1]
	punpckhdq xmm4, oword [eax+64]
	punpckhdq mm0, [0xfb5]
	punpckhdq xmm7, [0x37a]
