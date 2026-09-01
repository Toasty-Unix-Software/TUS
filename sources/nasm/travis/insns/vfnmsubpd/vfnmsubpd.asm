default rel
	vfnmsubpd xmm5, xmm1, xmm2, xmm2
	vfnmsubpd xmm5, xmm2, xmm2
	vfnmsubpd xmm5, xmm3, xmm1, xmm7
	vfnmsubpd ymm3, ymm6, ymm3, ymm4
	vfnmsubpd ymm3, ymm3, ymm4
	vfnmsubpd ymm5, ymm2, ymm6, ymm0
	vfnmsubpd xmm1, xmm3, xmm3, xmm4
	vfnmsubpd xmm1, xmm3, xmm4
	vfnmsubpd xmm6, xmm1, xmm4, xmm1
	vfnmsubpd ymm5, ymm0, ymm2, yword [0x31f]
	vfnmsubpd ymm5, ymm2, yword [0x31f]
	vfnmsubpd ymm5, ymm6, ymm2, ymm0
	vfnmsubpd xmm13, xmm9, xmm13, xmm12
	vfnmsubpd ymm12, ymm10, ymm13, ymm13
	vfnmsubpd xmm13, xmm11, xmm12, xmm11
	vfnmsubpd ymm12, ymm15, ymm8, ymm13
	vfnmsubpd xmm7, xmm4, oword [eax+1], xmm2
	vfnmsubpd xmm4, xmm1, oword [eax+64], xmm4
	vfnmsubpd ymm0, ymm7, yword [eax+1], ymm6
	vfnmsubpd ymm4, ymm5, yword [eax+64], ymm3
	vfnmsubpd xmm1, xmm1, xmm7, oword [eax+1]
	vfnmsubpd xmm3, xmm4, xmm6, oword [eax+64]
	vfnmsubpd ymm6, ymm0, ymm1, yword [eax+1]
	vfnmsubpd ymm0, ymm4, ymm7, yword [eax+64]
	vfnmsubpd xmm3, xmm0, [0x7f8], xmm7
	vfnmsubpd ymm2, ymm3, [0x94c], ymm2
	vfnmsubpd xmm6, xmm2, xmm7, [0x296]
	vfnmsubpd ymm3, ymm2, ymm3, [0x6c3]
