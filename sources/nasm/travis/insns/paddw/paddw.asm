default rel
	paddw mm4, mm5
	paddw mm2, qword [0x306]
	paddw xmm0, xmm4
	paddw xmm4, oword [0x28a]
	paddw xmm14, xmm9
	paddw mm0, qword [eax+1]
	paddw mm3, qword [eax+64]
	paddw xmm1, oword [eax+1]
	paddw xmm0, oword [eax+64]
	paddw mm2, [0xaf0]
	paddw xmm0, [0x7ae]
