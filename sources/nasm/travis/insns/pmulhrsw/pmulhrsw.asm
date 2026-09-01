default rel
	pmulhrsw mm0, mm7
	pmulhrsw mm3, qword [0xfb5]
	pmulhrsw xmm7, xmm4
	pmulhrsw xmm7, xmm2
	pmulhrsw xmm10, xmm10
	pmulhrsw mm5, qword [eax+1]
	pmulhrsw mm3, qword [eax+64]
	pmulhrsw xmm1, oword [eax+1]
	pmulhrsw xmm6, oword [eax+64]
	pmulhrsw mm7, [0x6bc]
	pmulhrsw xmm6, [0x38d]
