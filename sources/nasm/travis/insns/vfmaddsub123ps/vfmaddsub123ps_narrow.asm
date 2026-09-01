	vfmaddsub123ps xmm1, xmm0, xmm4
	vfmaddsub123ps xmm3, xmm7, xmm6
	vfmaddsub123ps ymm4, ymm7, ymm1
	vfmaddsub123ps ymm7, ymm5, yword [0xee0]

%ifdef ERROR
	vfmaddsub123ps xmm14, xmm15, xmm13
	vfmaddsub123ps ymm13, ymm10, ymm11
%endif
