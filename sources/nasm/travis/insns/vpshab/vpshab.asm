default rel
	vpshab xmm1, oword [0x7f1], xmm0
	vpshab xmm1, xmm0
	vpshab xmm4, xmm3, xmm5
	vpshab xmm4, xmm3, xmm3
	vpshab xmm4, xmm3
	vpshab xmm2, xmm0, xmm4
	vpshab xmm10, xmm10, xmm8
	vpshab xmm10, xmm15, xmm12
	vpshab xmm5, oword [eax+1], xmm7
	vpshab xmm7, oword [eax+64], xmm4
	vpshab xmm7, xmm5, oword [eax+1]
	vpshab xmm4, xmm0, oword [eax+64]
	vpshab xmm5, [0xe25], xmm2
	vpshab xmm2, xmm3, [0x680]
