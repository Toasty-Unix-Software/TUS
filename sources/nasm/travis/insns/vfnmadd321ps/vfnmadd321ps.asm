default rel
	vfnmadd321ps xmm0, xmm1, xmm3
	vfnmadd321ps xmm1, xmm4, xmm4
	vfnmadd321ps ymm2, ymm6, ymm6
	vfnmadd321ps ymm7, ymm2, ymm6
	vfnmadd321ps xmm8, xmm10, xmm9
	vfnmadd321ps ymm15, ymm11, ymm10
	vfnmadd321ps xmm6, xmm2, oword [eax+1]
	vfnmadd321ps xmm1, xmm6, oword [eax+64]
	vfnmadd321ps ymm4, ymm1, yword [eax+1]
	vfnmadd321ps ymm7, ymm0, yword [eax+64]
	vfnmadd321ps xmm3, xmm0, [0x8b0]
	vfnmadd321ps ymm6, ymm4, [0x617]
