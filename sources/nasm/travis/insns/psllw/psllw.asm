default rel
	psllw mm3, mm2
	psllw mm2, qword [0xf84]
	psllw mm2, 0x5a
	psllw mm5, 0x3c
	psllw xmm7, xmm7
	psllw xmm1, oword [0x11f]
	psllw xmm7, 0x7
	psllw xmm5, 0x5b
	psllw xmm13, xmm14
	psllw xmm9, 0x78
	psllw mm5, qword [eax+1]
	psllw mm6, qword [eax+64]
	psllw xmm0, oword [eax+1]
	psllw xmm0, oword [eax+64]
	psllw mm6, [0x5fa]
	psllw xmm2, [0x71d]
