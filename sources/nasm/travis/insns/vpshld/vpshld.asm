default rel
	vpshld xmm4, xmm4, xmm0
	vpshld xmm4, xmm0
	vpshld xmm6, xmm3, xmm3
	vpshld xmm2, xmm0, oword [0x980]
	vpshld xmm2, oword [0x980]
	vpshld xmm7, xmm5, xmm6
	vpshld xmm14, xmm14, xmm14
	vpshld xmm9, xmm10, xmm13
	vpshld xmm4, oword [eax+1], xmm1
	vpshld xmm7, oword [eax+64], xmm5
	vpshld xmm5, xmm2, oword [eax+1]
	vpshld xmm4, xmm3, oword [eax+64]
	vpshld xmm2, [0xfdf], xmm5
	vpshld xmm6, xmm1, [0x39a]
