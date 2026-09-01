	vfmaddsub231ph xmm3, xmm6, xmm4
	vfmaddsub231ph xmm3, xmm4
	vfmaddsub231ph xmm6, xmm6, oword [0x426]
	vfmaddsub231ph ymm3, ymm7, ymm1
	vfmaddsub231ph ymm3, ymm1
	vfmaddsub231ph ymm1, ymm3, ymm3
	vfmaddsub231ph zmm0, zmm4, zmm1
	vfmaddsub231ph zmm0, zmm1
	vfmaddsub231ph zmm3, zmm1, zword [0xf00]

%ifdef ERROR
	vfmaddsub231ph xmm14, xmm12, xmm11
	vfmaddsub231ph ymm9, ymm11, ymm12
	vfmaddsub231ph zmm13, zmm13, zmm13
	vfmaddsub231ph xmm22, xmm26, xmm21
	vfmaddsub231ph ymm24, ymm30, ymm25
	vfmaddsub231ph zmm22, zmm29, zmm20
%endif
