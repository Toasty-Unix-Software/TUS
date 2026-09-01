default rel
	psraw mm0, qword [0xef0]
	psraw mm0, mm6
	psraw mm6, 0x74
	psraw mm5, 0x65
	psraw xmm6, oword [0x775]
	psraw xmm7, xmm2
	psraw xmm1, 0x5f
	psraw xmm7, 0x21
	psraw xmm11, xmm14
	psraw xmm9, 0x46
	psraw mm7, qword [eax+1]
	psraw mm6, qword [eax+64]
	psraw xmm5, oword [eax+1]
	psraw xmm3, oword [eax+64]
	psraw mm4, [0x3e7]
	psraw xmm6, [0x4dd]
