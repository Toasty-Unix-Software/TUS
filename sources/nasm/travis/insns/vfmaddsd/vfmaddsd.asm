default rel
	vfmaddsd xmm2, xmm3, qword [0xded], xmm7
	vfmaddsd xmm2, qword [0xded], xmm7
	vfmaddsd xmm2, xmm3, xmm4, xmm3
	vfmaddsd xmm6, xmm1, xmm5, xmm5
	vfmaddsd xmm6, xmm5, xmm5
	vfmaddsd xmm7, xmm0, xmm0, xmm5
	vfmaddsd xmm10, xmm11, xmm14, xmm9
	vfmaddsd xmm14, xmm15, xmm13, xmm10
	vfmaddsd xmm3, xmm6, qword [eax+1], xmm6
	vfmaddsd xmm5, xmm3, qword [eax+64], xmm0
	vfmaddsd xmm3, xmm4, xmm3, qword [eax+1]
	vfmaddsd xmm7, xmm0, xmm6, qword [eax+64]
	vfmaddsd xmm1, xmm1, [0x67c], xmm4
	vfmaddsd xmm3, xmm0, xmm4, [0x9fe]
