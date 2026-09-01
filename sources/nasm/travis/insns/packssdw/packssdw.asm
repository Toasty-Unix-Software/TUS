default rel
	packssdw mm7, mm4
	packssdw mm6, mm0
	packssdw xmm1, xmm4
	packssdw xmm6, xmm2
	packssdw xmm13, xmm13
	packssdw mm4, qword [eax+1]
	packssdw mm4, qword [eax+64]
	packssdw xmm6, oword [eax+1]
	packssdw xmm3, oword [eax+64]
	packssdw mm3, [0x43c]
	packssdw xmm3, [0xee2]
