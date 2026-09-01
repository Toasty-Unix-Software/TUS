default rel
	vfnmadd213sd xmm5, xmm3, xmm1
	vfnmadd213sd xmm4, xmm5, xmm2
	vfnmadd213sd xmm5, xmm2, xmm1
	vfnmadd213sd xmm2, xmm1, qword [0xd15]
	vfnmadd213sd xmm12, xmm10, xmm9
	vfnmadd213sd xmm9, xmm15, xmm11
	vfnmadd213sd xmm21, xmm31, xmm19
	vfnmadd213sd xmm28, xmm31, xmm21
	vfnmadd213sd xmm2{k7}, xmm7, xmm1
	vfnmadd213sd xmm6{k2}{z}, xmm2, xmm1
	vfnmadd213sd xmm0, xmm1, xmm5, {rz-sae}
	vfnmadd213sd xmm2, xmm7, qword [eax+1]
	vfnmadd213sd xmm2, xmm5, qword [eax+64]
	vfnmadd213sd xmm3, xmm4, qword [eax+1]
	vfnmadd213sd xmm5, xmm7, qword [eax+64]
	vfnmadd213sd xmm1, xmm0, [0xcb3]
	vfnmadd213sd xmm5, xmm4, [0x5cf]
