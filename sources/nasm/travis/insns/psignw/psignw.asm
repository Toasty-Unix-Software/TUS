default rel
	psignw mm1, mm3
	psignw mm4, mm0
	psignw xmm5, xmm7
	psignw xmm6, oword [0xbc4]
	psignw xmm9, xmm8
	psignw mm7, qword [eax+1]
	psignw mm6, qword [eax+64]
	psignw xmm2, oword [eax+1]
	psignw xmm1, oword [eax+64]
	psignw mm6, [0x953]
	psignw xmm5, [0x10d]
