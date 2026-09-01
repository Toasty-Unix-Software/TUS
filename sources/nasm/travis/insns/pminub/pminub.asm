default rel
	pminub mm4, qword [0x25a]
	pminub mm0, mm3
	pminub xmm2, xmm2
	pminub xmm5, xmm3
	pminub xmm12, xmm14
	pminub mm0, qword [eax+1]
	pminub mm2, qword [eax+64]
	pminub xmm5, oword [eax+1]
	pminub xmm4, oword [eax+64]
	pminub mm5, [0xf99]
	pminub xmm7, [0x62a]
