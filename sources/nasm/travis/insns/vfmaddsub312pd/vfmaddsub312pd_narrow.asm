	vfmaddsub312pd xmm5, xmm4, xmm3
	vfmaddsub312pd xmm3, xmm4, xmm6
	vfmaddsub312pd ymm1, ymm0, ymm5
	vfmaddsub312pd ymm2, ymm5, ymm1

%ifdef ERROR
	vfmaddsub312pd xmm10, xmm9, xmm14
	vfmaddsub312pd ymm14, ymm11, ymm15
%endif
