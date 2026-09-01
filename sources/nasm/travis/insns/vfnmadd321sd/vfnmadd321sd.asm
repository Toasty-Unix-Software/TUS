default rel
	vfnmadd321sd xmm5, xmm7, xmm6
	vfnmadd321sd xmm2, xmm4, qword [0xef7]
	vfnmadd321sd xmm14, xmm13, xmm11
	vfnmadd321sd xmm4, xmm4, qword [eax+1]
	vfnmadd321sd xmm1, xmm2, qword [eax+64]
	vfnmadd321sd xmm5, xmm5, [0xfe8]
