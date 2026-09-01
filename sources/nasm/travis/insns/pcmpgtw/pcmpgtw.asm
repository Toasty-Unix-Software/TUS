default rel
	pcmpgtw mm0, mm2
	pcmpgtw mm5, mm2
	pcmpgtw xmm1, oword [0x150]
	pcmpgtw xmm1, xmm0
	pcmpgtw xmm15, xmm8
	pcmpgtw mm3, qword [eax+1]
	pcmpgtw mm2, qword [eax+64]
	pcmpgtw xmm5, oword [eax+1]
	pcmpgtw xmm5, oword [eax+64]
	pcmpgtw mm1, [0x79e]
	pcmpgtw xmm4, [0x519]
