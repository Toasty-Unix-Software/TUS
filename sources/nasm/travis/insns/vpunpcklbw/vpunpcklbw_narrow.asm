	vpunpcklbw xmm0, xmm3, oword [0x912]
	vpunpcklbw xmm0, oword [0x912]
	vpunpcklbw xmm6, xmm7, xmm1
	vpunpcklbw ymm0, ymm6, ymm6
	vpunpcklbw ymm0, ymm6
	vpunpcklbw ymm0, ymm1, yword [0x8ed]
	vpunpcklbw xmm1, xmm6, xmm4
	vpunpcklbw xmm1, xmm4
	vpunpcklbw xmm0, xmm4, oword [0x39d]
	vpunpcklbw ymm0, ymm7, ymm2
	vpunpcklbw ymm0, ymm2
	vpunpcklbw ymm7, ymm2, yword [0x370]

%ifdef ERROR
	vpunpcklbw xmm10, xmm8, xmm13
	vpunpcklbw ymm12, ymm13, ymm8
	vpunpcklbw xmm14, xmm12, xmm12
	vpunpcklbw ymm12, ymm11, ymm12
	vpunpcklbw xmm16, xmm23, xmm17
	vpunpcklbw ymm20, ymm24, ymm21
	vpunpcklbw xmm22, xmm19, xmm21
	vpunpcklbw ymm26, ymm29, ymm25
%endif
