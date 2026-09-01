	vfmaddsub231ps xmm6, xmm2, xmm5
	vfmaddsub231ps xmm0, xmm3, oword [0x320]
	vfmaddsub231ps ymm5, ymm7, yword [0xf9a]
	vfmaddsub231ps ymm4, ymm5, ymm6
	vfmaddsub231ps xmm7, xmm4, xmm1
	vfmaddsub231ps xmm7, xmm4, xmm3
	vfmaddsub231ps ymm1, ymm4, ymm5
	vfmaddsub231ps ymm0, ymm6, ymm3

%ifdef ERROR
	vfmaddsub231ps xmm14, xmm14, xmm8
	vfmaddsub231ps ymm11, ymm10, ymm11
	vfmaddsub231ps xmm13, xmm10, xmm8
	vfmaddsub231ps ymm14, ymm11, ymm9
	vfmaddsub231ps xmm21, xmm22, xmm25
	vfmaddsub231ps ymm17, ymm16, ymm23
	vfmaddsub231ps xmm28, xmm24, xmm23
	vfmaddsub231ps ymm29, ymm25, ymm25
%endif
