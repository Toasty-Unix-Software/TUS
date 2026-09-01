default rel
	vfnmadd312ps xmm0, xmm6, oword [0x189]
	vfnmadd312ps xmm2, xmm4, xmm7
	vfnmadd312ps ymm4, ymm7, ymm3
	vfnmadd312ps ymm4, ymm3, ymm7
	vfnmadd312ps xmm13, xmm10, xmm8
	vfnmadd312ps ymm9, ymm12, ymm9
	vfnmadd312ps xmm7, xmm1, oword [eax+1]
	vfnmadd312ps xmm3, xmm1, oword [eax+64]
	vfnmadd312ps ymm1, ymm3, yword [eax+1]
	vfnmadd312ps ymm6, ymm5, yword [eax+64]
	vfnmadd312ps xmm0, xmm4, [0x930]
	vfnmadd312ps ymm4, ymm7, [0x48c]
