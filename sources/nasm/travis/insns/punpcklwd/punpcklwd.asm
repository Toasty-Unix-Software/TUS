default rel
	punpcklwd mm0, mm7
	punpcklwd mm6, qword [0x35f]
	punpcklwd xmm0, xmm0
	punpcklwd xmm3, xmm1
	punpcklwd xmm12, xmm13
	punpcklwd mm2, qword [eax+1]
	punpcklwd mm2, qword [eax+64]
	punpcklwd xmm6, oword [eax+1]
	punpcklwd xmm3, oword [eax+64]
	punpcklwd mm7, [0xeee]
	punpcklwd xmm2, [0xd8c]
