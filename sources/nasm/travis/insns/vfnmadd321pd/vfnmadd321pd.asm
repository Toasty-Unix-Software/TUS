default rel
	vfnmadd321pd xmm5, xmm4, oword [0x28c]
	vfnmadd321pd xmm7, xmm3, xmm4
	vfnmadd321pd ymm4, ymm1, ymm2
	vfnmadd321pd ymm1, ymm5, ymm3
	vfnmadd321pd xmm8, xmm9, xmm15
	vfnmadd321pd ymm12, ymm14, ymm11
	vfnmadd321pd xmm4, xmm3, oword [eax+1]
	vfnmadd321pd xmm5, xmm1, oword [eax+64]
	vfnmadd321pd ymm7, ymm1, yword [eax+1]
	vfnmadd321pd ymm5, ymm5, yword [eax+64]
	vfnmadd321pd xmm3, xmm2, [0xfc0]
	vfnmadd321pd ymm0, ymm6, [0x75c]
