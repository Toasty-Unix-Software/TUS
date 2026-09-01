default rel
	vfmsubadd312pd xmm0, xmm4, xmm2
	vfmsubadd312pd xmm0, xmm7, xmm5
	vfmsubadd312pd ymm4, ymm6, ymm6
	vfmsubadd312pd ymm7, ymm0, ymm6
	vfmsubadd312pd xmm11, xmm12, xmm11
	vfmsubadd312pd ymm9, ymm12, ymm11
	vfmsubadd312pd xmm2, xmm7, oword [eax+1]
	vfmsubadd312pd xmm4, xmm3, oword [eax+64]
	vfmsubadd312pd ymm1, ymm0, yword [eax+1]
	vfmsubadd312pd ymm1, ymm7, yword [eax+64]
	vfmsubadd312pd xmm0, xmm7, [0x467]
	vfmsubadd312pd ymm5, ymm3, [0xf55]
