default rel
	vfmadd312pd xmm7, xmm1, xmm3
	vfmadd312pd xmm2, xmm7, xmm2
	vfmadd312pd ymm2, ymm3, ymm2
	vfmadd312pd ymm0, ymm7, ymm3
	vfmadd312pd xmm14, xmm15, xmm9
	vfmadd312pd ymm15, ymm15, ymm15
	vfmadd312pd xmm2, xmm1, oword [eax+1]
	vfmadd312pd xmm3, xmm4, oword [eax+64]
	vfmadd312pd ymm2, ymm5, yword [eax+1]
	vfmadd312pd ymm7, ymm7, yword [eax+64]
	vfmadd312pd xmm7, xmm4, [0x65a]
	vfmadd312pd ymm6, ymm7, [0x589]
