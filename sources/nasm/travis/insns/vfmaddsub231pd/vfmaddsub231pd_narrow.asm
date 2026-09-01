	vfmaddsub231pd xmm4, xmm1, xmm2
	vfmaddsub231pd xmm0, xmm1, xmm6
	vfmaddsub231pd ymm1, ymm5, yword [0x2ff]
	vfmaddsub231pd ymm5, ymm1, ymm0
	vfmaddsub231pd xmm6, xmm3, xmm5
	vfmaddsub231pd xmm1, xmm6, xmm0
	vfmaddsub231pd ymm3, ymm3, ymm3
	vfmaddsub231pd ymm3, ymm4, ymm0

%ifdef ERROR
	vfmaddsub231pd xmm15, xmm11, xmm11
	vfmaddsub231pd ymm13, ymm12, ymm10
	vfmaddsub231pd xmm13, xmm12, xmm12
	vfmaddsub231pd ymm9, ymm10, ymm14
	vfmaddsub231pd xmm18, xmm26, xmm20
	vfmaddsub231pd ymm28, ymm22, ymm31
	vfmaddsub231pd xmm17, xmm29, xmm25
	vfmaddsub231pd ymm16, ymm25, ymm21
%endif
