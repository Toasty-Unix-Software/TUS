default rel
	punpckhbw mm2, mm4
	punpckhbw mm0, mm1
	punpckhbw xmm4, xmm1
	punpckhbw xmm4, xmm7
	punpckhbw xmm15, xmm10
	punpckhbw mm6, qword [eax+1]
	punpckhbw mm1, qword [eax+64]
	punpckhbw xmm0, oword [eax+1]
	punpckhbw xmm1, oword [eax+64]
	punpckhbw mm3, [0x807]
	punpckhbw xmm5, [0x8e3]
