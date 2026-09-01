default rel
	vfmadd213sh xmm5, xmm5, xmm2
	vfmadd213sh xmm5, xmm2
	vfmadd213sh xmm2, xmm3, xmm0
	vfmadd213sh xmm12, xmm10, xmm9
	vfmadd213sh xmm23, xmm29, xmm25
	vfmadd213sh xmm4{k2}, xmm7, xmm3
	vfmadd213sh xmm1{k6}{z}, xmm3, word [0xba6]
	vfmadd213sh xmm6, xmm1, xmm1, {rd-sae}
	vfmadd213sh xmm3, xmm1, word [eax+1]
	vfmadd213sh xmm6, xmm1, word [eax+64]
	vfmadd213sh xmm7, xmm2, [0xa7d]
