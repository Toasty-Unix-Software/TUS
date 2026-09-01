default rel
	vfmsubadd123pd xmm7, xmm0, oword [0xc3c]
	vfmsubadd123pd xmm1, xmm2, xmm7
	vfmsubadd123pd ymm3, ymm7, yword [0x96c]
	vfmsubadd123pd ymm0, ymm2, ymm5
	vfmsubadd123pd xmm8, xmm13, xmm11
	vfmsubadd123pd ymm9, ymm10, ymm8
	vfmsubadd123pd xmm1, xmm2, oword [eax+1]
	vfmsubadd123pd xmm0, xmm1, oword [eax+64]
	vfmsubadd123pd ymm6, ymm2, yword [eax+1]
	vfmsubadd123pd ymm5, ymm3, yword [eax+64]
	vfmsubadd123pd xmm3, xmm6, [0xc1f]
	vfmsubadd123pd ymm4, ymm3, [0x9c8]
