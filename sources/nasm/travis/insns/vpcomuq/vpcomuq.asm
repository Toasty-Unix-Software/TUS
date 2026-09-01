default rel
	vpcomuq xmm3, xmm6, xmm4, 0xfb
	vpcomuq xmm3, xmm4, 0xfb
	vpcomuq xmm2, xmm2, xmm7, 0xba
	vpcomuq xmm12, xmm11, xmm8, 0x4
	vpcomuq xmm0, xmm4, oword [eax+1], 0x10
	vpcomuq xmm6, xmm3, oword [eax+64], 0xde
	vpcomuq xmm0, xmm2, [0x23e], 0xfb
