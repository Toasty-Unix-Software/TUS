	vfmadd321ps xmm1, xmm2, xmm5
	vfmadd321ps xmm7, xmm1, xmm1
	vfmadd321ps ymm2, ymm1, ymm4
	vfmadd321ps ymm6, ymm2, ymm2

%ifdef ERROR
	vfmadd321ps xmm8, xmm15, xmm8
	vfmadd321ps ymm11, ymm12, ymm14
%endif
