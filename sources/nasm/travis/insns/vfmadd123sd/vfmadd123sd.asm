default rel
	vfmadd123sd xmm0, xmm4, qword [0xcd5]
	vfmadd123sd xmm2, xmm0, qword [0xd37]
	vfmadd123sd xmm13, xmm8, xmm11
	vfmadd123sd xmm3, xmm2, qword [eax+1]
	vfmadd123sd xmm7, xmm3, qword [eax+64]
	vfmadd123sd xmm2, xmm1, [0x4c7]
