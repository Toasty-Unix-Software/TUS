default rel
	vdivsh xmm1, xmm3, xmm2
	vdivsh xmm1, xmm2
	vdivsh xmm2, xmm0, xmm1
	vdivsh xmm13, xmm8, xmm9
	vdivsh xmm24, xmm30, xmm18
	vdivsh xmm6{k5}, xmm1, word [0xda9]
	vdivsh xmm4{k7}{z}, xmm2, xmm1
	vdivsh xmm2, xmm3, xmm4, {rd-sae}
	vdivsh xmm3, xmm4, word [eax+1]
	vdivsh xmm6, xmm5, word [eax+64]
	vdivsh xmm5, xmm3, [0x52f]
