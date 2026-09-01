default rel
	vrsqrtsh xmm4, xmm0, xmm2
	vrsqrtsh xmm4, xmm2
	vrsqrtsh xmm4, xmm2, xmm4
	vrsqrtsh xmm11, xmm13, xmm11
	vrsqrtsh xmm19, xmm22, xmm31
	vrsqrtsh xmm4{k4}, xmm2, xmm1
	vrsqrtsh xmm4{k1}{z}, xmm4, xmm2
	vrsqrtsh xmm3, xmm7, xmm1, {sae}
	vrsqrtsh xmm4, xmm2, word [eax+1]
	vrsqrtsh xmm4, xmm6, word [eax+64]
	vrsqrtsh xmm1, xmm1, [0xe7f]
