default rel
	pmaddubsw mm3, qword [0x365]
	pmaddubsw mm2, mm1
	pmaddubsw xmm7, oword [0x9a7]
	pmaddubsw xmm3, xmm5
	pmaddubsw xmm9, xmm15
	pmaddubsw mm1, qword [eax+1]
	pmaddubsw mm7, qword [eax+64]
	pmaddubsw xmm4, oword [eax+1]
	pmaddubsw xmm3, oword [eax+64]
	pmaddubsw mm4, [0xa14]
	pmaddubsw xmm4, [0x8c3]
