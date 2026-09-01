default rel
	phaddd mm0, mm2
	phaddd mm1, qword [0x7f6]
	phaddd xmm5, oword [0x1b8]
	phaddd xmm3, xmm1
	phaddd xmm9, xmm11
	phaddd mm5, qword [eax+1]
	phaddd mm1, qword [eax+64]
	phaddd xmm6, oword [eax+1]
	phaddd xmm2, oword [eax+64]
	phaddd mm1, [0xa96]
	phaddd xmm1, [0x658]
