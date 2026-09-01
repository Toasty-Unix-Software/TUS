default rel
	punpckhwd mm5, mm6
	punpckhwd mm0, mm3
	punpckhwd xmm7, oword [0x692]
	punpckhwd xmm4, xmm3
	punpckhwd xmm8, xmm12
	punpckhwd mm3, qword [eax+1]
	punpckhwd mm7, qword [eax+64]
	punpckhwd xmm7, oword [eax+1]
	punpckhwd xmm4, oword [eax+64]
	punpckhwd mm6, [0x455]
	punpckhwd xmm3, [0x69a]
