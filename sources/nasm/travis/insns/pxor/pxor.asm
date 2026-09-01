default rel
	pxor mm7, qword [0xb3e]
	pxor mm1, mm7
	pxor xmm2, xmm4
	pxor xmm0, xmm6
	pxor xmm14, xmm13
	pxor mm4, qword [eax+1]
	pxor mm4, qword [eax+64]
	pxor xmm1, oword [eax+1]
	pxor xmm2, oword [eax+64]
	pxor mm2, [0x7c8]
	pxor xmm2, [0x790]
