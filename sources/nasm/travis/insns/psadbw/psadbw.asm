default rel
	psadbw mm6, mm3
	psadbw mm5, mm1
	psadbw xmm4, oword [0x275]
	psadbw xmm4, xmm3
	psadbw xmm15, xmm12
	psadbw mm1, qword [eax+1]
	psadbw mm7, qword [eax+64]
	psadbw xmm0, oword [eax+1]
	psadbw xmm4, oword [eax+64]
	psadbw mm2, [0x137]
	psadbw xmm0, [0x2d3]
