default rel
	psubsb mm1, mm0
	psubsb mm7, mm5
	psubsb xmm0, xmm2
	psubsb xmm7, oword [0xb27]
	psubsb xmm12, xmm12
	psubsb mm6, qword [eax+1]
	psubsb mm2, qword [eax+64]
	psubsb xmm5, oword [eax+1]
	psubsb xmm5, oword [eax+64]
	psubsb mm0, [0xded]
	psubsb xmm3, [0xedb]
