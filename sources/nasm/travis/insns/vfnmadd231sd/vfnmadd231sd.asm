default rel
	vfnmadd231sd xmm3, xmm4, xmm3
	vfnmadd231sd xmm3, xmm3, xmm5
	vfnmadd231sd xmm6, xmm6, xmm1
	vfnmadd231sd xmm3, xmm3, qword [0x165]
	vfnmadd231sd xmm13, xmm12, xmm15
	vfnmadd231sd xmm15, xmm11, xmm13
	vfnmadd231sd xmm30, xmm17, xmm18
	vfnmadd231sd xmm27, xmm18, xmm18
	vfnmadd231sd xmm2{k5}, xmm4, xmm7
	vfnmadd231sd xmm3{k3}{z}, xmm0, qword [0x47e]
	vfnmadd231sd xmm1, xmm2, xmm4, {rz-sae}
	vfnmadd231sd xmm1, xmm7, qword [eax+1]
	vfnmadd231sd xmm3, xmm7, qword [eax+64]
	vfnmadd231sd xmm4, xmm6, qword [eax+1]
	vfnmadd231sd xmm4, xmm2, qword [eax+64]
	vfnmadd231sd xmm4, xmm7, [0xcea]
	vfnmadd231sd xmm5, xmm3, [0x956]
