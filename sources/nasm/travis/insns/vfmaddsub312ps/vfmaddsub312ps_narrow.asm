	vfmaddsub312ps xmm7, xmm7, xmm1
	vfmaddsub312ps xmm6, xmm7, xmm5
	vfmaddsub312ps ymm4, ymm5, yword [0xcba]
	vfmaddsub312ps ymm3, ymm5, ymm2

%ifdef ERROR
	vfmaddsub312ps xmm13, xmm14, xmm13
	vfmaddsub312ps ymm14, ymm10, ymm9
%endif
