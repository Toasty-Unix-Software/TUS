default rel
	vfnmsub312sd xmm5, xmm7, xmm2
	vfnmsub312sd xmm1, xmm5, xmm0
	vfnmsub312sd xmm9, xmm12, xmm13
	vfnmsub312sd xmm4, xmm6, qword [eax+1]
	vfnmsub312sd xmm0, xmm2, qword [eax+64]
	vfnmsub312sd xmm3, xmm1, [0x8fa]
