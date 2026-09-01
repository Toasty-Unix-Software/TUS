default rel
	vsubsh xmm3, xmm6, xmm3
	vsubsh xmm3, xmm3
	vsubsh xmm3, xmm2, word [0xcb7]
	vsubsh xmm12, xmm13, xmm11
	vsubsh xmm30, xmm28, xmm17
	vsubsh xmm4{k1}, xmm5, xmm6
	vsubsh xmm3{k5}{z}, xmm5, word [0xb9f]
	vsubsh xmm5, xmm3, xmm5, {rd-sae}
	vsubsh xmm4, xmm4, word [eax+1]
	vsubsh xmm6, xmm1, word [eax+64]
	vsubsh xmm4, xmm7, [0x7e2]
