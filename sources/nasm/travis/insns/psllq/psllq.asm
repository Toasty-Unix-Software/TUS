default rel
	psllq mm6, mm3
	psllq mm6, mm4
	psllq mm1, 0x54
	psllq mm7, 0x40
	psllq xmm1, xmm2
	psllq xmm2, xmm0
	psllq xmm3, 0x30
	psllq xmm3, 0x61
	psllq xmm8, xmm10
	psllq xmm12, 0x35
	psllq mm3, qword [eax+1]
	psllq mm1, qword [eax+64]
	psllq xmm7, oword [eax+1]
	psllq xmm3, oword [eax+64]
	psllq mm2, [0x2f0]
	psllq xmm4, [0x262]
