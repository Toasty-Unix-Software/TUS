default rel
	pmullw mm7, mm1
	pmullw mm3, mm2
	pmullw xmm4, oword [0x4ef]
	pmullw xmm5, oword [0x512]
	pmullw xmm8, xmm14
	pmullw mm3, qword [eax+1]
	pmullw mm6, qword [eax+64]
	pmullw xmm7, oword [eax+1]
	pmullw xmm1, oword [eax+64]
	pmullw mm1, [0x40a]
	pmullw xmm5, [0xef9]
