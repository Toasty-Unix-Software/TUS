default rel
	vfmadd123pd xmm7, xmm1, xmm1
	vfmadd123pd xmm2, xmm7, xmm4
	vfmadd123pd ymm6, ymm7, ymm2
	vfmadd123pd ymm2, ymm1, ymm0
	vfmadd123pd xmm15, xmm8, xmm14
	vfmadd123pd ymm14, ymm15, ymm15
	vfmadd123pd xmm2, xmm6, oword [eax+1]
	vfmadd123pd xmm0, xmm0, oword [eax+64]
	vfmadd123pd ymm2, ymm3, yword [eax+1]
	vfmadd123pd ymm6, ymm1, yword [eax+64]
	vfmadd123pd xmm1, xmm7, [0xf61]
	vfmadd123pd ymm3, ymm1, [0x775]
