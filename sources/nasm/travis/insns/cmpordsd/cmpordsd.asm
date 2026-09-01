default rel
	cmpordsd xmm4, xmm5
	cmpordsd xmm0, xmm0
	cmpordsd xmm13, xmm13
	cmpordsd xmm1, oword [eax+1]
	cmpordsd xmm2, oword [eax+64]
	cmpordsd xmm0, [0xcaf]
