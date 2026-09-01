default rel
	cmpordps xmm5, xmm3
	cmpordps xmm4, xmm0
	cmpordps xmm12, xmm13
	cmpordps xmm3, oword [eax+1]
	cmpordps xmm6, oword [eax+64]
	cmpordps xmm4, [0xf09]
