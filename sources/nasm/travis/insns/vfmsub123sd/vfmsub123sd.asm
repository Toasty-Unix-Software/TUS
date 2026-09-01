default rel
	vfmsub123sd xmm0, xmm5, xmm7
	vfmsub123sd xmm4, xmm1, xmm2
	vfmsub123sd xmm14, xmm13, xmm13
	vfmsub123sd xmm0, xmm7, qword [eax+1]
	vfmsub123sd xmm4, xmm5, qword [eax+64]
	vfmsub123sd xmm3, xmm4, [0x30f]
