default rel
	vfmadd321ps xmm1, xmm2, xmm5
	vfmadd321ps xmm7, xmm1, xmm1
	vfmadd321ps ymm2, ymm1, ymm4
	vfmadd321ps ymm6, ymm2, ymm2
	vfmadd321ps xmm8, xmm15, xmm8
	vfmadd321ps ymm11, ymm12, ymm14
	vfmadd321ps xmm0, xmm3, oword [eax+1]
	vfmadd321ps xmm4, xmm4, oword [eax+64]
	vfmadd321ps ymm2, ymm1, yword [eax+1]
	vfmadd321ps ymm0, ymm5, yword [eax+64]
	vfmadd321ps xmm4, xmm2, [0x8e5]
	vfmadd321ps ymm2, ymm3, [0x641]
