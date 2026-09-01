default rel
	vfmsubadd321pd xmm1, xmm0, xmm3
	vfmsubadd321pd xmm3, xmm3, xmm7
	vfmsubadd321pd ymm5, ymm3, ymm3
	vfmsubadd321pd ymm3, ymm7, ymm7
	vfmsubadd321pd xmm14, xmm11, xmm14
	vfmsubadd321pd ymm11, ymm13, ymm14
	vfmsubadd321pd xmm0, xmm1, oword [eax+1]
	vfmsubadd321pd xmm5, xmm6, oword [eax+64]
	vfmsubadd321pd ymm3, ymm4, yword [eax+1]
	vfmsubadd321pd ymm5, ymm7, yword [eax+64]
	vfmsubadd321pd xmm5, xmm7, [0x1bc]
	vfmsubadd321pd ymm7, ymm5, [0xf28]
