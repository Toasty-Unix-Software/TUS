default rel
	vpmacssdd xmm5, xmm0, oword [0xaee], xmm6
	vpmacssdd xmm5, oword [0xaee], xmm6
	vpmacssdd xmm3, xmm5, oword [0xf99], xmm0
	vpmacssdd xmm11, xmm11, xmm13, xmm14
	vpmacssdd xmm7, xmm1, oword [eax+1], xmm0
	vpmacssdd xmm0, xmm6, oword [eax+64], xmm6
	vpmacssdd xmm0, xmm6, [0xfff], xmm3
