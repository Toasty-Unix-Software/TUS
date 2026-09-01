default rel
	pshufb mm4, qword [0x5d4]
	pshufb mm6, qword [0x626]
	pshufb xmm1, oword [0xe64]
	pshufb xmm1, oword [0x24c]
	pshufb xmm8, xmm10
	pshufb mm5, qword [eax+1]
	pshufb mm1, qword [eax+64]
	pshufb xmm0, oword [eax+1]
	pshufb xmm7, oword [eax+64]
	pshufb mm2, [0xa2f]
	pshufb xmm3, [0x171]
