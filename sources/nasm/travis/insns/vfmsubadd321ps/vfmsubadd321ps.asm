default rel
	vfmsubadd321ps xmm0, xmm1, oword [0x2cb]
	vfmsubadd321ps xmm7, xmm3, oword [0x670]
	vfmsubadd321ps ymm0, ymm6, yword [0x961]
	vfmsubadd321ps ymm3, ymm4, ymm1
	vfmsubadd321ps xmm8, xmm8, xmm8
	vfmsubadd321ps ymm15, ymm11, ymm12
	vfmsubadd321ps xmm6, xmm4, oword [eax+1]
	vfmsubadd321ps xmm5, xmm1, oword [eax+64]
	vfmsubadd321ps ymm3, ymm0, yword [eax+1]
	vfmsubadd321ps ymm3, ymm1, yword [eax+64]
	vfmsubadd321ps xmm5, xmm7, [0xa5f]
	vfmsubadd321ps ymm6, ymm4, [0x573]
