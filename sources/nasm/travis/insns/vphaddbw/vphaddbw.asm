default rel
	vphaddbw xmm6, xmm2
	vphaddbw xmm6
	vphaddbw xmm7, xmm2
	vphaddbw xmm8, xmm14
	vphaddbw xmm0, oword [eax+1]
	vphaddbw xmm5, oword [eax+64]
	vphaddbw xmm5, [0x78c]
