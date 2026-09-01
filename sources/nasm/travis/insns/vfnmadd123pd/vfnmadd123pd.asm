default rel
	vfnmadd123pd xmm0, xmm2, oword [0x5a1]
	vfnmadd123pd xmm5, xmm7, xmm0
	vfnmadd123pd ymm4, ymm2, ymm0
	vfnmadd123pd ymm6, ymm1, ymm3
	vfnmadd123pd xmm10, xmm8, xmm15
	vfnmadd123pd ymm12, ymm13, ymm10
	vfnmadd123pd xmm4, xmm5, oword [eax+1]
	vfnmadd123pd xmm6, xmm7, oword [eax+64]
	vfnmadd123pd ymm6, ymm4, yword [eax+1]
	vfnmadd123pd ymm1, ymm5, yword [eax+64]
	vfnmadd123pd xmm5, xmm1, [0xa44]
	vfnmadd123pd ymm6, ymm2, [0x355]
