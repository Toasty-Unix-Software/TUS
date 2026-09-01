default rel
	pand mm4, mm3
	pand mm5, qword [0x40b]
	pand xmm5, xmm2
	pand xmm2, xmm6
	pand xmm10, xmm15
	pand mm5, qword [eax+1]
	pand mm6, qword [eax+64]
	pand xmm3, oword [eax+1]
	pand xmm1, oword [eax+64]
	pand mm6, [0x185]
	pand xmm4, [0xc22]
