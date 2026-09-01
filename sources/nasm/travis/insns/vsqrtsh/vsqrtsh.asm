default rel
	vsqrtsh xmm6, xmm6, xmm4
	vsqrtsh xmm6, xmm4
	vsqrtsh xmm6, xmm2, word [0x669]
	vsqrtsh xmm9, xmm13, xmm9
	vsqrtsh xmm22, xmm29, xmm25
	vsqrtsh xmm1{k6}, xmm4, word [0xf04]
	vsqrtsh xmm6{k5}{z}, xmm4, xmm7
	vsqrtsh xmm4, xmm7, xmm2, {ru-sae}
	vsqrtsh xmm2, xmm1, word [eax+1]
	vsqrtsh xmm5, xmm4, word [eax+64]
	vsqrtsh xmm7, xmm1, [0xd95]
