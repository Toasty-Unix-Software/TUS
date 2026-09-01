default rel
	pavgw mm2, mm0
	pavgw mm3, mm3
	pavgw xmm6, xmm1
	pavgw xmm4, xmm1
	pavgw xmm12, xmm15
	pavgw mm4, qword [eax+1]
	pavgw mm6, qword [eax+64]
	pavgw xmm7, oword [eax+1]
	pavgw xmm3, oword [eax+64]
	pavgw mm1, [0xf2b]
	pavgw xmm6, [0xd4a]
