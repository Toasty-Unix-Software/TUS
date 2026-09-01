default rel
	vpmacssww xmm0, xmm4, xmm2, xmm4
	vpmacssww xmm0, xmm2, xmm4
	vpmacssww xmm7, xmm2, xmm7, xmm3
	vpmacssww xmm15, xmm15, xmm10, xmm12
	vpmacssww xmm6, xmm0, oword [eax+1], xmm6
	vpmacssww xmm6, xmm0, oword [eax+64], xmm3
	vpmacssww xmm0, xmm4, [0xfb8], xmm2
