default rel
	vfmadd132sh xmm1, xmm6, word [0x201]
	vfmadd132sh xmm1, word [0x201]
	vfmadd132sh xmm3, xmm0, word [0x7fa]
	vfmadd132sh xmm10, xmm9, xmm9
	vfmadd132sh xmm24, xmm27, xmm16
	vfmadd132sh xmm7{k7}, xmm7, xmm2
	vfmadd132sh xmm0{k5}{z}, xmm4, word [0xa55]
	vfmadd132sh xmm5, xmm1, xmm1, {rd-sae}
	vfmadd132sh xmm1, xmm4, word [eax+1]
	vfmadd132sh xmm7, xmm5, word [eax+64]
	vfmadd132sh xmm7, xmm6, [0x2b4]
