default rel
	vfmsub321sd xmm2, xmm7, xmm2
	vfmsub321sd xmm0, xmm3, xmm3
	vfmsub321sd xmm11, xmm14, xmm8
	vfmsub321sd xmm3, xmm5, qword [eax+1]
	vfmsub321sd xmm6, xmm0, qword [eax+64]
	vfmsub321sd xmm2, xmm2, [0x675]
