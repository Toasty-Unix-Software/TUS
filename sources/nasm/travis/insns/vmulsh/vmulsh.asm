default rel
	vmulsh xmm3, xmm0, xmm1
	vmulsh xmm3, xmm1
	vmulsh xmm4, xmm7, word [0xa91]
	vmulsh xmm9, xmm14, xmm8
	vmulsh xmm23, xmm22, xmm17
	vmulsh xmm6{k1}, xmm4, word [0x1ef]
	vmulsh xmm3{k4}{z}, xmm4, xmm3
	vmulsh xmm1, xmm6, xmm6, {rn-sae}
	vmulsh xmm3, xmm3, word [eax+1]
	vmulsh xmm6, xmm2, word [eax+64]
	vmulsh xmm3, xmm1, [0x934]
