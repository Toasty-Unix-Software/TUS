default rel
	cmpeqsd xmm2, oword [0xe2e]
	cmpeqsd xmm2, xmm3
	cmpeqsd xmm11, xmm13
	cmpeqsd xmm2, oword [eax+1]
	cmpeqsd xmm6, oword [eax+64]
	cmpeqsd xmm4, [0x6db]
