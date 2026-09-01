default rel
	vfmadd321sd xmm2, xmm5, qword [0xab2]
	vfmadd321sd xmm1, xmm6, xmm0
	vfmadd321sd xmm13, xmm15, xmm15
	vfmadd321sd xmm5, xmm1, qword [eax+1]
	vfmadd321sd xmm5, xmm4, qword [eax+64]
	vfmadd321sd xmm4, xmm0, [0x52d]
