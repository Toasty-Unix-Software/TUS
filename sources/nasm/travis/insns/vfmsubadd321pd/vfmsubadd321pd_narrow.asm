	vfmsubadd321pd xmm1, xmm0, xmm3
	vfmsubadd321pd xmm3, xmm3, xmm7
	vfmsubadd321pd ymm5, ymm3, ymm3
	vfmsubadd321pd ymm3, ymm7, ymm7

%ifdef ERROR
	vfmsubadd321pd xmm14, xmm11, xmm14
	vfmsubadd321pd ymm11, ymm13, ymm14
%endif
