default rel
	vpshad xmm4, oword [0x556], xmm7
	vpshad xmm4, xmm7
	vpshad xmm3, oword [0x667], xmm3
	vpshad xmm6, xmm2, xmm1
	vpshad xmm6, xmm1
	vpshad xmm0, xmm7, xmm2
	vpshad xmm13, xmm14, xmm13
	vpshad xmm9, xmm10, xmm9
	vpshad xmm3, oword [eax+1], xmm5
	vpshad xmm0, oword [eax+64], xmm6
	vpshad xmm7, xmm5, oword [eax+1]
	vpshad xmm4, xmm5, oword [eax+64]
	vpshad xmm0, [0x5b3], xmm0
	vpshad xmm7, xmm5, [0x254]
