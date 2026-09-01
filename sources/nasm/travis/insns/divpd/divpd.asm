default rel
	divpd xmm7, xmm3
	divpd xmm3, xmm5
	divpd xmm10, xmm8
	divpd xmm4, oword [eax+1]
	divpd xmm0, oword [eax+64]
	divpd xmm4, [0xd89]
