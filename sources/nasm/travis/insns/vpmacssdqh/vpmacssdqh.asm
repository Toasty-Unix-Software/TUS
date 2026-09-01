default rel
	vpmacssdqh xmm1, xmm4, xmm2, xmm7
	vpmacssdqh xmm1, xmm2, xmm7
	vpmacssdqh xmm0, xmm5, oword [0x26f], xmm3
	vpmacssdqh xmm10, xmm12, xmm8, xmm14
	vpmacssdqh xmm4, xmm0, oword [eax+1], xmm4
	vpmacssdqh xmm6, xmm0, oword [eax+64], xmm5
	vpmacssdqh xmm7, xmm2, [0xff5], xmm5
