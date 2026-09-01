default rel
	vroundpd xmm3, oword [0x827], 0x40
	vroundpd xmm4, xmm3, 0x51
	vroundpd ymm1, ymm3, 0x9a
	vroundpd ymm3, yword [0x434], 0x11
	vroundpd xmm13, xmm14, 0x66
	vroundpd ymm9, ymm15, 0xa9
	vroundpd xmm4, oword [eax+1], 0xc
	vroundpd xmm2, oword [eax+64], 0x57
	vroundpd ymm3, yword [eax+1], 0xa3
	vroundpd ymm5, yword [eax+64], 0x2a
	vroundpd xmm4, [0xa20], 0x6f
	vroundpd ymm1, [0xef5], 0xc4
