default rel
	vfnmadd231sh xmm1, xmm5, word [0xeb5]
	vfnmadd231sh xmm1, word [0xeb5]
	vfnmadd231sh xmm3, xmm5, xmm0
	vfnmadd231sh xmm8, xmm10, xmm15
	vfnmadd231sh xmm24, xmm22, xmm28
	vfnmadd231sh xmm0{k6}, xmm5, word [0x57f]
	vfnmadd231sh xmm4{k6}{z}, xmm7, xmm4
	vfnmadd231sh xmm0, xmm1, xmm1, {rd-sae}
	vfnmadd231sh xmm7, xmm7, word [eax+1]
	vfnmadd231sh xmm0, xmm6, word [eax+64]
	vfnmadd231sh xmm1, xmm0, [0xcaa]
