default rel
	movdqu xmm6, xmm4
	movdqu xmm6, xmm0
	movdqu xmm0, xmm2
	movdqu xmm0, xmm2
	movdqu xmm14, xmm12
	movdqu xmm10, xmm9
	movdqu xmm0, oword [eax+1]
	movdqu xmm6, oword [eax+64]
	movdqu oword [eax+1], xmm1
	movdqu oword [eax+64], xmm4
	movdqu xmm1, [0xe4e]
	movdqu [0xc30], xmm4
