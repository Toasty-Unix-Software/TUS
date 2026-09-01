default rel
	vfmsubadd123ps xmm6, xmm5, oword [0xb04]
	vfmsubadd123ps xmm5, xmm1, xmm1
	vfmsubadd123ps ymm4, ymm7, yword [0x7e1]
	vfmsubadd123ps ymm6, ymm7, ymm7
	vfmsubadd123ps xmm10, xmm14, xmm10
	vfmsubadd123ps ymm14, ymm8, ymm15
	vfmsubadd123ps xmm1, xmm4, oword [eax+1]
	vfmsubadd123ps xmm3, xmm7, oword [eax+64]
	vfmsubadd123ps ymm0, ymm2, yword [eax+1]
	vfmsubadd123ps ymm2, ymm1, yword [eax+64]
	vfmsubadd123ps xmm7, xmm2, [0xb70]
	vfmsubadd123ps ymm4, ymm0, [0xa2f]
