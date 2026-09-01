default rel
	vpmacsdd xmm2, xmm3, xmm2, xmm0
	vpmacsdd xmm2, xmm2, xmm0
	vpmacsdd xmm7, xmm0, xmm4, xmm2
	vpmacsdd xmm8, xmm12, xmm14, xmm8
	vpmacsdd xmm3, xmm2, oword [eax+1], xmm7
	vpmacsdd xmm4, xmm3, oword [eax+64], xmm5
	vpmacsdd xmm5, xmm0, [0x931], xmm5
