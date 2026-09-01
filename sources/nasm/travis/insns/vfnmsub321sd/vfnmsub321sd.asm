default rel
	vfnmsub321sd xmm0, xmm5, xmm4
	vfnmsub321sd xmm4, xmm6, xmm1
	vfnmsub321sd xmm15, xmm8, xmm11
	vfnmsub321sd xmm5, xmm0, qword [eax+1]
	vfnmsub321sd xmm5, xmm3, qword [eax+64]
	vfnmsub321sd xmm3, xmm6, [0x547]
