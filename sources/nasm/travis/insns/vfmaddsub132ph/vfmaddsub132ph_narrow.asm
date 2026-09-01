	vfmaddsub132ph xmm4, xmm7, xmm0
	vfmaddsub132ph xmm4, xmm0
	vfmaddsub132ph xmm1, xmm7, xmm6
	vfmaddsub132ph ymm0, ymm7, ymm5
	vfmaddsub132ph ymm0, ymm5
	vfmaddsub132ph ymm3, ymm5, yword [0xd6d]
	vfmaddsub132ph zmm6, zmm4, zword [0xa0b]
	vfmaddsub132ph zmm6, zword [0xa0b]
	vfmaddsub132ph zmm6, zmm6, zmm6

%ifdef ERROR
	vfmaddsub132ph xmm11, xmm8, xmm15
	vfmaddsub132ph ymm15, ymm15, ymm9
	vfmaddsub132ph zmm13, zmm9, zmm11
	vfmaddsub132ph xmm24, xmm26, xmm30
	vfmaddsub132ph ymm21, ymm27, ymm29
	vfmaddsub132ph zmm20, zmm22, zmm22
%endif
