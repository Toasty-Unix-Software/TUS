default rel
	pmulhw mm6, mm1
	pmulhw mm5, mm1
	pmulhw xmm1, xmm6
	pmulhw xmm3, oword [0x302]
	pmulhw xmm10, xmm15
	pmulhw mm6, qword [eax+1]
	pmulhw mm6, qword [eax+64]
	pmulhw xmm2, oword [eax+1]
	pmulhw xmm3, oword [eax+64]
	pmulhw mm2, [0x614]
	pmulhw xmm1, [0xf76]
