default rel
	pmulhuw mm3, mm0
	pmulhuw mm6, mm3
	pmulhuw xmm5, xmm2
	pmulhuw xmm1, oword [0x6a0]
	pmulhuw xmm9, xmm10
	pmulhuw mm7, qword [eax+1]
	pmulhuw mm0, qword [eax+64]
	pmulhuw xmm3, oword [eax+1]
	pmulhuw xmm6, oword [eax+64]
	pmulhuw mm5, [0x2f4]
	pmulhuw xmm2, [0x902]
