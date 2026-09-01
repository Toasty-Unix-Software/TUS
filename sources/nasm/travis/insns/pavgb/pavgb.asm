default rel
	pavgb mm7, qword [0x880]
	pavgb mm0, mm7
	pavgb xmm2, xmm0
	pavgb xmm4, oword [0xac6]
	pavgb xmm9, xmm8
	pavgb mm0, qword [eax+1]
	pavgb mm0, qword [eax+64]
	pavgb xmm5, oword [eax+1]
	pavgb xmm2, oword [eax+64]
	pavgb mm2, [0x1f4]
	pavgb xmm0, [0x987]
