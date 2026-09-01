default rel
	vfmsubadd312ps xmm3, xmm1, xmm6
	vfmsubadd312ps xmm3, xmm4, oword [0xbbc]
	vfmsubadd312ps ymm5, ymm5, ymm6
	vfmsubadd312ps ymm3, ymm0, yword [0x64b]
	vfmsubadd312ps xmm8, xmm8, xmm8
	vfmsubadd312ps ymm12, ymm10, ymm10
	vfmsubadd312ps xmm5, xmm3, oword [eax+1]
	vfmsubadd312ps xmm1, xmm7, oword [eax+64]
	vfmsubadd312ps ymm2, ymm0, yword [eax+1]
	vfmsubadd312ps ymm3, ymm0, yword [eax+64]
	vfmsubadd312ps xmm3, xmm5, [0x55a]
	vfmsubadd312ps ymm7, ymm3, [0x5d7]
