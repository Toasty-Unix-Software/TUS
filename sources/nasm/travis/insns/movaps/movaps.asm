default rel
	movaps xmm7, xmm1
	movaps xmm4, oword [0xda7]
	movaps xmm5, xmm1
	movaps xmm4, xmm5
	movaps xmm13, xmm14
	movaps xmm13, xmm10
	movaps xmm3, oword [eax+1]
	movaps xmm6, oword [eax+64]
	movaps oword [eax+1], xmm7
	movaps oword [eax+64], xmm7
	movaps xmm3, [0x2bc]
	movaps [0xdaa], xmm3
