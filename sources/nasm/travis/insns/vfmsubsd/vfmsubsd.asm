default rel
	vfmsubsd xmm3, xmm1, xmm3, xmm6
	vfmsubsd xmm3, xmm3, xmm6
	vfmsubsd xmm0, xmm5, qword [0xade], xmm0
	vfmsubsd xmm7, xmm4, xmm2, xmm3
	vfmsubsd xmm7, xmm2, xmm3
	vfmsubsd xmm2, xmm1, xmm1, xmm3
	vfmsubsd xmm8, xmm10, xmm9, xmm11
	vfmsubsd xmm12, xmm11, xmm9, xmm11
	vfmsubsd xmm7, xmm1, qword [eax+1], xmm4
	vfmsubsd xmm1, xmm4, qword [eax+64], xmm6
	vfmsubsd xmm5, xmm7, xmm1, qword [eax+1]
	vfmsubsd xmm1, xmm4, xmm2, qword [eax+64]
	vfmsubsd xmm4, xmm4, [0xe55], xmm3
	vfmsubsd xmm0, xmm6, xmm1, [0x30c]
