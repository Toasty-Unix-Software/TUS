default rel
	vaddsh xmm7, xmm5, xmm4
	vaddsh xmm7, xmm4
	vaddsh xmm3, xmm1, word [0xaba]
	vaddsh xmm11, xmm14, xmm13
	vaddsh xmm21, xmm30, xmm17
	vaddsh xmm3{k4}, xmm7, xmm3
	vaddsh xmm0{k1}{z}, xmm1, word [0x75a]
	vaddsh xmm5, xmm6, xmm4, {rz-sae}
	vaddsh xmm2, xmm2, word [eax+1]
	vaddsh xmm3, xmm7, word [eax+64]
	vaddsh xmm0, xmm3, [0xc95]
