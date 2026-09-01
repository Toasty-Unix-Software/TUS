default rel
	cmpeqpd xmm7, xmm6
	cmpeqpd xmm7, xmm5
	cmpeqpd xmm9, xmm13
	cmpeqpd xmm1, oword [eax+1]
	cmpeqpd xmm6, oword [eax+64]
	cmpeqpd xmm3, [0x310]
