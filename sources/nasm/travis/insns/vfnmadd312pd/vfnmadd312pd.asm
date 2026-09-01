default rel
	vfnmadd312pd xmm3, xmm2, oword [0x3b3]
	vfnmadd312pd xmm3, xmm0, xmm0
	vfnmadd312pd ymm3, ymm2, ymm5
	vfnmadd312pd ymm0, ymm1, ymm5
	vfnmadd312pd xmm15, xmm9, xmm15
	vfnmadd312pd ymm9, ymm14, ymm13
	vfnmadd312pd xmm0, xmm1, oword [eax+1]
	vfnmadd312pd xmm3, xmm6, oword [eax+64]
	vfnmadd312pd ymm4, ymm2, yword [eax+1]
	vfnmadd312pd ymm6, ymm1, yword [eax+64]
	vfnmadd312pd xmm4, xmm4, [0x817]
	vfnmadd312pd ymm5, ymm5, [0xfa4]
