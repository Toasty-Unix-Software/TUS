default rel
	vfmulcsh xmm2, xmm3, xmm1
	vfmulcsh xmm2, xmm1
	vfmulcsh xmm2, xmm1, xmm4
	vfmulcsh xmm9, xmm13, xmm11
	vfmulcsh xmm21, xmm24, xmm28
	vfmulcsh xmm0{k1}, xmm3, dword [0x634]
	vfmulcsh xmm5{k4}{z}, xmm6, xmm1
	vfmulcsh xmm5, xmm2, xmm0, {rz-sae}
	vfmulcsh xmm3, xmm6, dword [eax+1]
	vfmulcsh xmm0, xmm1, dword [eax+64]
	vfmulcsh xmm7, xmm0, [0xcec]
