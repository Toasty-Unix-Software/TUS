default rel
	vfmadd231sh xmm0, xmm6, xmm0
	vfmadd231sh xmm0, xmm0
	vfmadd231sh xmm4, xmm1, xmm5
	vfmadd231sh xmm8, xmm10, xmm11
	vfmadd231sh xmm16, xmm29, xmm27
	vfmadd231sh xmm4{k5}, xmm3, xmm0
	vfmadd231sh xmm3{k4}{z}, xmm7, xmm4
	vfmadd231sh xmm1, xmm4, xmm7, {ru-sae}
	vfmadd231sh xmm3, xmm7, word [eax+1]
	vfmadd231sh xmm2, xmm6, word [eax+64]
	vfmadd231sh xmm3, xmm4, [0x11d]
