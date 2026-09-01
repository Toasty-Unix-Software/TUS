default rel
	vfnmaddsd xmm6, xmm1, xmm2, xmm1
	vfnmaddsd xmm6, xmm2, xmm1
	vfnmaddsd xmm7, xmm7, xmm7, xmm3
	vfnmaddsd xmm2, xmm6, xmm1, xmm6
	vfnmaddsd xmm2, xmm1, xmm6
	vfnmaddsd xmm7, xmm5, xmm5, xmm7
	vfnmaddsd xmm11, xmm14, xmm13, xmm14
	vfnmaddsd xmm9, xmm12, xmm9, xmm12
	vfnmaddsd xmm2, xmm7, qword [eax+1], xmm0
	vfnmaddsd xmm1, xmm5, qword [eax+64], xmm5
	vfnmaddsd xmm7, xmm0, xmm4, qword [eax+1]
	vfnmaddsd xmm0, xmm2, xmm2, qword [eax+64]
	vfnmaddsd xmm5, xmm7, [0x1c1], xmm3
	vfnmaddsd xmm0, xmm4, xmm4, [0xbc2]
