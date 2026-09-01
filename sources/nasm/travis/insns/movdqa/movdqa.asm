default rel
	movdqa xmm6, xmm2
	movdqa xmm2, xmm5
	movdqa oword [0x4a9], xmm7
	movdqa oword [0x56b], xmm0
	movdqa xmm8, xmm14
	movdqa xmm11, xmm11
	movdqa xmm2, oword [eax+1]
	movdqa xmm3, oword [eax+64]
	movdqa oword [eax+1], xmm0
	movdqa oword [eax+64], xmm1
	movdqa xmm5, [0xc47]
	movdqa [0x3f5], xmm4
