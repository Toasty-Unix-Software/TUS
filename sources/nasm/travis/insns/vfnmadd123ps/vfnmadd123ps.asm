default rel
	vfnmadd123ps xmm5, xmm0, xmm4
	vfnmadd123ps xmm3, xmm2, xmm1
	vfnmadd123ps ymm0, ymm5, yword [0x57b]
	vfnmadd123ps ymm4, ymm5, yword [0x3c6]
	vfnmadd123ps xmm11, xmm8, xmm12
	vfnmadd123ps ymm14, ymm13, ymm15
	vfnmadd123ps xmm4, xmm0, oword [eax+1]
	vfnmadd123ps xmm2, xmm5, oword [eax+64]
	vfnmadd123ps ymm1, ymm6, yword [eax+1]
	vfnmadd123ps ymm3, ymm2, yword [eax+64]
	vfnmadd123ps xmm6, xmm2, [0x5cc]
	vfnmadd123ps ymm3, ymm6, [0x1e0]
