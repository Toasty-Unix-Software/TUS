	vfmaddsub321pd xmm4, xmm1, xmm4
	vfmaddsub321pd xmm6, xmm2, xmm3
	vfmaddsub321pd ymm3, ymm4, ymm5
	vfmaddsub321pd ymm0, ymm5, ymm4

%ifdef ERROR
	vfmaddsub321pd xmm8, xmm11, xmm14
	vfmaddsub321pd ymm8, ymm15, ymm9
%endif
