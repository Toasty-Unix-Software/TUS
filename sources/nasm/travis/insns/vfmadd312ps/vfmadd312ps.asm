default rel
	vfmadd312ps xmm6, xmm0, xmm3
	vfmadd312ps xmm0, xmm6, xmm3
	vfmadd312ps ymm1, ymm1, yword [0x54b]
	vfmadd312ps ymm4, ymm4, yword [0xdf4]
	vfmadd312ps xmm8, xmm12, xmm8
	vfmadd312ps ymm11, ymm15, ymm9
	vfmadd312ps xmm2, xmm5, oword [eax+1]
	vfmadd312ps xmm4, xmm0, oword [eax+64]
	vfmadd312ps ymm0, ymm1, yword [eax+1]
	vfmadd312ps ymm1, ymm5, yword [eax+64]
	vfmadd312ps xmm3, xmm6, [0x9b0]
	vfmadd312ps ymm1, ymm4, [0x957]
