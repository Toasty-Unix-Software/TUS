default rel
	vgetexpsh xmm5, xmm2, xmm5
	vgetexpsh xmm6, xmm2, xmm2
	vgetexpsh xmm14, xmm15, xmm11
	vgetexpsh xmm19, xmm31, xmm20
	vgetexpsh xmm1{k1}, xmm0, word [0xb44]
	vgetexpsh xmm7{k7}{z}, xmm5, xmm7
	vgetexpsh xmm1, xmm2, xmm3, {sae}
	vgetexpsh xmm3, xmm2, word [eax+1]
	vgetexpsh xmm4, xmm2, word [eax+64]
	vgetexpsh xmm4, xmm4, [0xddc]
