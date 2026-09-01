default rel
	vfnmadd123sd xmm3, xmm3, xmm2
	vfnmadd123sd xmm1, xmm6, xmm1
	vfnmadd123sd xmm12, xmm11, xmm13
	vfnmadd123sd xmm6, xmm0, qword [eax+1]
	vfnmadd123sd xmm0, xmm0, qword [eax+64]
	vfnmadd123sd xmm7, xmm3, [0xd4f]
