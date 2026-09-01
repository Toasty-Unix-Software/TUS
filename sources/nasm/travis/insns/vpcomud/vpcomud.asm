default rel
	vpcomud xmm7, xmm4, oword [0x7a4], 0x62
	vpcomud xmm7, oword [0x7a4], 0x62
	vpcomud xmm0, xmm7, xmm5, 0xac
	vpcomud xmm10, xmm9, xmm12, 0xb2
	vpcomud xmm3, xmm4, oword [eax+1], 0xf9
	vpcomud xmm3, xmm7, oword [eax+64], 0x81
	vpcomud xmm3, xmm1, [0x9b2], 0x3a
