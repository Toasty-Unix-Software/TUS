	vpunpckhbw xmm6, xmm1, xmm4
	vpunpckhbw xmm6, xmm4
	vpunpckhbw xmm1, xmm3, xmm1
	vpunpckhbw ymm4, ymm2, yword [0x830]
	vpunpckhbw ymm4, yword [0x830]
	vpunpckhbw ymm0, ymm3, ymm1
	vpunpckhbw xmm4, xmm7, xmm6
	vpunpckhbw xmm4, xmm6
	vpunpckhbw xmm1, xmm3, xmm0
	vpunpckhbw ymm7, ymm3, ymm1
	vpunpckhbw ymm7, ymm1
	vpunpckhbw ymm3, ymm1, yword [0x5f8]

%ifdef ERROR
	vpunpckhbw xmm11, xmm15, xmm9
	vpunpckhbw ymm14, ymm13, ymm8
	vpunpckhbw xmm13, xmm14, xmm13
	vpunpckhbw ymm8, ymm15, ymm10
	vpunpckhbw xmm27, xmm23, xmm20
	vpunpckhbw ymm29, ymm23, ymm24
	vpunpckhbw xmm22, xmm21, xmm21
	vpunpckhbw ymm16, ymm27, ymm20
%endif
