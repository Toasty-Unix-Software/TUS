default rel
	paddsb mm3, mm2
	paddsb mm7, mm5
	paddsb xmm0, xmm4
	paddsb xmm5, oword [0x4ae]
	paddsb xmm11, xmm14
	paddsb mm3, qword [eax+1]
	paddsb mm0, qword [eax+64]
	paddsb xmm4, oword [eax+1]
	paddsb xmm7, oword [eax+64]
	paddsb mm0, [0xf8e]
	paddsb xmm5, [0xb27]
