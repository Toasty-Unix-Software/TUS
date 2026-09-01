default rel
	vpmacsdqh xmm3, xmm4, xmm4, xmm6
	vpmacsdqh xmm3, xmm4, xmm6
	vpmacsdqh xmm0, xmm4, oword [0x216], xmm0
	vpmacsdqh xmm13, xmm15, xmm11, xmm12
	vpmacsdqh xmm3, xmm2, oword [eax+1], xmm2
	vpmacsdqh xmm7, xmm7, oword [eax+64], xmm7
	vpmacsdqh xmm1, xmm4, [0x48d], xmm7
