default rel
	vroundsd xmm1, xmm7, xmm6, 0xfd
	vroundsd xmm1, xmm6, 0xfd
	vroundsd xmm5, xmm0, xmm3, 0x80
	vroundsd xmm10, xmm11, xmm11, 0xfe
	vroundsd xmm6, xmm2, qword [eax+1], 0x85
	vroundsd xmm2, xmm1, qword [eax+64], 0xca
	vroundsd xmm0, xmm5, [0x7b3], 0x7
