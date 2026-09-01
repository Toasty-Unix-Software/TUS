default rel
	vfnmsubsd xmm4, xmm1, xmm1, xmm7
	vfnmsubsd xmm4, xmm1, xmm7
	vfnmsubsd xmm4, xmm0, xmm3, xmm2
	vfnmsubsd xmm3, xmm4, xmm3, qword [0x49b]
	vfnmsubsd xmm3, xmm3, qword [0x49b]
	vfnmsubsd xmm5, xmm6, xmm6, xmm7
	vfnmsubsd xmm11, xmm15, xmm15, xmm12
	vfnmsubsd xmm9, xmm9, xmm9, xmm10
	vfnmsubsd xmm0, xmm0, qword [eax+1], xmm6
	vfnmsubsd xmm4, xmm3, qword [eax+64], xmm2
	vfnmsubsd xmm0, xmm7, xmm4, qword [eax+1]
	vfnmsubsd xmm0, xmm2, xmm0, qword [eax+64]
	vfnmsubsd xmm1, xmm4, [0x12f], xmm6
	vfnmsubsd xmm4, xmm1, xmm7, [0x32a]
