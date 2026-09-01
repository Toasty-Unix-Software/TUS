default rel
	vfmadd213sd xmm6, xmm5, xmm7
	vfmadd213sd xmm4, xmm1, xmm0
	vfmadd213sd xmm7, xmm3, qword [0x12e]
	vfmadd213sd xmm4, xmm7, xmm4
	vfmadd213sd xmm9, xmm9, xmm14
	vfmadd213sd xmm11, xmm9, xmm13
	vfmadd213sd xmm30, xmm27, xmm16
	vfmadd213sd xmm17, xmm26, xmm27
	vfmadd213sd xmm5{k1}, xmm3, qword [0x25b]
	vfmadd213sd xmm7{k7}{z}, xmm2, xmm7
	vfmadd213sd xmm2, xmm2, xmm0, {rz-sae}
	vfmadd213sd xmm6, xmm2, qword [eax+1]
	vfmadd213sd xmm1, xmm5, qword [eax+64]
	vfmadd213sd xmm3, xmm6, qword [eax+1]
	vfmadd213sd xmm7, xmm4, qword [eax+64]
	vfmadd213sd xmm2, xmm1, [0xb9b]
	vfmadd213sd xmm7, xmm6, [0x174]
