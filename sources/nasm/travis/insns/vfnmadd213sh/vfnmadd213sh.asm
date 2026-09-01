default rel
	vfnmadd213sh xmm2, xmm3, xmm4
	vfnmadd213sh xmm2, xmm4
	vfnmadd213sh xmm4, xmm6, word [0x3b6]
	vfnmadd213sh xmm12, xmm9, xmm14
	vfnmadd213sh xmm20, xmm18, xmm24
	vfnmadd213sh xmm6{k1}, xmm0, xmm0
	vfnmadd213sh xmm7{k3}{z}, xmm1, xmm4
	vfnmadd213sh xmm2, xmm7, xmm1, {rn-sae}
	vfnmadd213sh xmm3, xmm3, word [eax+1]
	vfnmadd213sh xmm6, xmm2, word [eax+64]
	vfnmadd213sh xmm1, xmm1, [0xc26]
