default rel
	paddsw mm5, mm3
	paddsw mm5, mm5
	paddsw xmm5, oword [0xff4]
	paddsw xmm1, xmm4
	paddsw xmm8, xmm9
	paddsw mm1, qword [eax+1]
	paddsw mm1, qword [eax+64]
	paddsw xmm4, oword [eax+1]
	paddsw xmm4, oword [eax+64]
	paddsw mm7, [0x99f]
	paddsw xmm7, [0x636]
