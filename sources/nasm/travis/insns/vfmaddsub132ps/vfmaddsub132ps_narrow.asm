	vfmaddsub132ps xmm0, xmm0, xmm4
	vfmaddsub132ps xmm3, xmm4, xmm5
	vfmaddsub132ps ymm1, ymm4, yword [0xe42]
	vfmaddsub132ps ymm4, ymm1, yword [0xf1b]
	vfmaddsub132ps xmm2, xmm1, xmm3
	vfmaddsub132ps xmm3, xmm0, xmm1
	vfmaddsub132ps ymm7, ymm0, yword [0xd72]
	vfmaddsub132ps ymm7, ymm4, ymm6

%ifdef ERROR
	vfmaddsub132ps xmm10, xmm14, xmm13
	vfmaddsub132ps ymm14, ymm11, ymm15
	vfmaddsub132ps xmm10, xmm15, xmm11
	vfmaddsub132ps ymm10, ymm9, ymm11
	vfmaddsub132ps xmm17, xmm16, xmm17
	vfmaddsub132ps ymm31, ymm17, ymm25
	vfmaddsub132ps xmm29, xmm31, xmm18
	vfmaddsub132ps ymm26, ymm22, ymm19
%endif
