default rel
	vfmadd321pd xmm5, xmm0, xmm2
	vfmadd321pd xmm6, xmm0, xmm7
	vfmadd321pd ymm2, ymm2, yword [0xb9b]
	vfmadd321pd ymm1, ymm3, ymm7
	vfmadd321pd xmm15, xmm13, xmm8
	vfmadd321pd ymm10, ymm12, ymm9
	vfmadd321pd xmm5, xmm5, oword [eax+1]
	vfmadd321pd xmm2, xmm4, oword [eax+64]
	vfmadd321pd ymm2, ymm3, yword [eax+1]
	vfmadd321pd ymm7, ymm4, yword [eax+64]
	vfmadd321pd xmm4, xmm3, [0x6ea]
	vfmadd321pd ymm2, ymm4, [0xf16]
