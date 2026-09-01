	vfmsubadd321ps xmm0, xmm1, oword [0x2cb]
	vfmsubadd321ps xmm7, xmm3, oword [0x670]
	vfmsubadd321ps ymm0, ymm6, yword [0x961]
	vfmsubadd321ps ymm3, ymm4, ymm1

%ifdef ERROR
	vfmsubadd321ps xmm8, xmm8, xmm8
	vfmsubadd321ps ymm15, ymm11, ymm12
%endif
