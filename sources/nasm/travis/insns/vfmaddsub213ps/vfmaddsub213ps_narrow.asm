	vfmaddsub213ps xmm3, xmm1, oword [0xf24]
	vfmaddsub213ps xmm6, xmm7, xmm7
	vfmaddsub213ps ymm7, ymm1, yword [0x8a0]
	vfmaddsub213ps ymm1, ymm4, ymm5
	vfmaddsub213ps xmm1, xmm1, xmm5
	vfmaddsub213ps xmm1, xmm5, xmm4
	vfmaddsub213ps ymm3, ymm7, ymm0
	vfmaddsub213ps ymm0, ymm1, yword [0xefb]

%ifdef ERROR
	vfmaddsub213ps xmm13, xmm8, xmm9
	vfmaddsub213ps ymm11, ymm12, ymm13
	vfmaddsub213ps xmm15, xmm10, xmm11
	vfmaddsub213ps ymm15, ymm11, ymm11
	vfmaddsub213ps xmm22, xmm30, xmm17
	vfmaddsub213ps ymm26, ymm29, ymm27
	vfmaddsub213ps xmm22, xmm17, xmm20
	vfmaddsub213ps ymm27, ymm31, ymm26
%endif
