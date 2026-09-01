default rel
	pmaxub mm2, mm0
	pmaxub mm6, mm1
	pmaxub xmm4, xmm1
	pmaxub xmm4, xmm5
	pmaxub xmm11, xmm12
	pmaxub mm0, qword [eax+1]
	pmaxub mm4, qword [eax+64]
	pmaxub xmm4, oword [eax+1]
	pmaxub xmm1, oword [eax+64]
	pmaxub mm5, [0xf1d]
	pmaxub xmm7, [0x60c]
