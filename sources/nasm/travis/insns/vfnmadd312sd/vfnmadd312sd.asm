default rel
	vfnmadd312sd xmm4, xmm4, xmm5
	vfnmadd312sd xmm2, xmm6, qword [0x4db]
	vfnmadd312sd xmm12, xmm13, xmm8
	vfnmadd312sd xmm2, xmm2, qword [eax+1]
	vfnmadd312sd xmm4, xmm7, qword [eax+64]
	vfnmadd312sd xmm7, xmm4, [0x423]
