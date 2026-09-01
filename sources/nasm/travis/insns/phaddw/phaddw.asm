default rel
	phaddw mm7, qword [0x5e5]
	phaddw mm0, qword [0x32a]
	phaddw xmm4, xmm0
	phaddw xmm2, oword [0x59d]
	phaddw xmm8, xmm12
	phaddw mm1, qword [eax+1]
	phaddw mm7, qword [eax+64]
	phaddw xmm0, oword [eax+1]
	phaddw xmm2, oword [eax+64]
	phaddw mm7, [0xec6]
	phaddw xmm0, [0x527]
