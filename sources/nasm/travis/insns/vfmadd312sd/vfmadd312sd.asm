default rel
	vfmadd312sd xmm4, xmm4, xmm0
	vfmadd312sd xmm2, xmm7, qword [0x817]
	vfmadd312sd xmm10, xmm13, xmm8
	vfmadd312sd xmm5, xmm6, qword [eax+1]
	vfmadd312sd xmm7, xmm0, qword [eax+64]
	vfmadd312sd xmm7, xmm5, [0x283]
