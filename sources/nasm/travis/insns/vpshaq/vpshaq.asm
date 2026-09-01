default rel
	vpshaq xmm6, xmm0, xmm5
	vpshaq xmm6, xmm5
	vpshaq xmm7, xmm0, xmm7
	vpshaq xmm1, xmm7, oword [0x436]
	vpshaq xmm1, oword [0x436]
	vpshaq xmm6, xmm1, xmm3
	vpshaq xmm10, xmm10, xmm8
	vpshaq xmm13, xmm9, xmm8
	vpshaq xmm0, oword [eax+1], xmm6
	vpshaq xmm6, oword [eax+64], xmm1
	vpshaq xmm5, xmm4, oword [eax+1]
	vpshaq xmm4, xmm2, oword [eax+64]
	vpshaq xmm4, [0xc09], xmm6
	vpshaq xmm4, xmm6, [0xe44]
