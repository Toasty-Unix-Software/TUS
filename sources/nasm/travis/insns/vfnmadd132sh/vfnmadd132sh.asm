default rel
	vfnmadd132sh xmm3, xmm7, word [0x79a]
	vfnmadd132sh xmm3, word [0x79a]
	vfnmadd132sh xmm7, xmm5, word [0xf46]
	vfnmadd132sh xmm15, xmm13, xmm8
	vfnmadd132sh xmm30, xmm28, xmm30
	vfnmadd132sh xmm3{k5}, xmm7, xmm2
	vfnmadd132sh xmm6{k7}{z}, xmm7, xmm4
	vfnmadd132sh xmm1, xmm4, xmm6, {rd-sae}
	vfnmadd132sh xmm7, xmm0, word [eax+1]
	vfnmadd132sh xmm1, xmm3, word [eax+64]
	vfnmadd132sh xmm2, xmm1, [0x9a7]
