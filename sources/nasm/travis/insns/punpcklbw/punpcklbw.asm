default rel
	punpcklbw mm5, mm5
	punpcklbw mm6, qword [0xc84]
	punpcklbw xmm2, oword [0x204]
	punpcklbw xmm2, xmm4
	punpcklbw xmm15, xmm15
	punpcklbw mm4, qword [eax+1]
	punpcklbw mm6, qword [eax+64]
	punpcklbw xmm5, oword [eax+1]
	punpcklbw xmm7, oword [eax+64]
	punpcklbw mm0, [0x6d8]
	punpcklbw xmm0, [0xd55]
