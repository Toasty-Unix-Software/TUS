default rel
	orpd xmm4, xmm0
	orpd xmm0, xmm4
	orpd xmm11, xmm14
	orpd xmm1, oword [eax+1]
	orpd xmm0, oword [eax+64]
	orpd xmm6, [0xd70]
