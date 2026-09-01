default rel
	pblendw xmm0, xmm1, 0xab
	pblendw xmm5, oword [0xe8c], 0x7b
	pblendw xmm12, xmm11, 0x18
	pblendw xmm1, oword [eax+1], 0x30
	pblendw xmm2, oword [eax+64], 0xa0
	pblendw xmm3, [0xaed], 0xcc
