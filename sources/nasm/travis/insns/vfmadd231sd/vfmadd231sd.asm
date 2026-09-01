default rel
	vfmadd231sd xmm5, xmm2, xmm4
	vfmadd231sd xmm5, xmm5, xmm7
	vfmadd231sd xmm5, xmm3, xmm2
	vfmadd231sd xmm5, xmm7, xmm3
	vfmadd231sd xmm11, xmm9, xmm9
	vfmadd231sd xmm10, xmm9, xmm12
	vfmadd231sd xmm22, xmm16, xmm24
	vfmadd231sd xmm29, xmm21, xmm21
	vfmadd231sd xmm4{k6}, xmm0, xmm0
	vfmadd231sd xmm3{k3}{z}, xmm1, xmm7
	vfmadd231sd xmm3, xmm1, xmm0, {ru-sae}
	vfmadd231sd xmm7, xmm4, qword [eax+1]
	vfmadd231sd xmm5, xmm3, qword [eax+64]
	vfmadd231sd xmm0, xmm6, qword [eax+1]
	vfmadd231sd xmm3, xmm4, qword [eax+64]
	vfmadd231sd xmm0, xmm2, [0x5e6]
	vfmadd231sd xmm4, xmm7, [0x966]
