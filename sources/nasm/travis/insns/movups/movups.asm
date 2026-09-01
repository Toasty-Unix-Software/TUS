default rel
	movups xmm5, xmm7
	movups xmm3, oword [0xa2f]
	movups oword [0xee1], xmm6
	movups xmm2, xmm7
	movups xmm14, xmm13
	movups xmm8, xmm8
	movups xmm4, oword [eax+1]
	movups xmm6, oword [eax+64]
	movups oword [eax+1], xmm5
	movups oword [eax+64], xmm1
	movups xmm4, [0x442]
	movups [0xc2c], xmm6
