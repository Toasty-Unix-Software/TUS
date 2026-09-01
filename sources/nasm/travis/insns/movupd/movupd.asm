default rel
	movupd xmm0, xmm6
	movupd xmm2, xmm5
	movupd xmm0, xmm0
	movupd xmm5, xmm4
	movupd xmm12, xmm10
	movupd xmm12, xmm12
	movupd xmm3, oword [eax+1]
	movupd xmm0, oword [eax+64]
	movupd oword [eax+1], xmm3
	movupd oword [eax+64], xmm3
	movupd xmm0, [0x8f7]
	movupd [0x607], xmm2
