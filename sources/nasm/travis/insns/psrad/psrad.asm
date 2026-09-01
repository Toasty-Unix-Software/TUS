default rel
	psrad mm3, qword [0x228]
	psrad mm6, mm2
	psrad mm2, 0x7f
	psrad mm0, 0x4
	psrad xmm5, oword [0x9fa]
	psrad xmm7, xmm4
	psrad xmm2, 0x43
	psrad xmm6, 0x34
	psrad xmm12, xmm8
	psrad xmm12, 0x33
	psrad mm2, qword [eax+1]
	psrad mm6, qword [eax+64]
	psrad xmm5, oword [eax+1]
	psrad xmm1, oword [eax+64]
	psrad mm4, [0xe72]
	psrad xmm5, [0xfdc]
