	vfnmadd321ps xmm0, xmm1, xmm3
	vfnmadd321ps xmm1, xmm4, xmm4
	vfnmadd321ps ymm2, ymm6, ymm6
	vfnmadd321ps ymm7, ymm2, ymm6

%ifdef ERROR
	vfnmadd321ps xmm8, xmm10, xmm9
	vfnmadd321ps ymm15, ymm11, ymm10
%endif
