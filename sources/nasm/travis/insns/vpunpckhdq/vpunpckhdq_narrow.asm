	vpunpckhdq xmm4, xmm1, xmm0
	vpunpckhdq xmm4, xmm0
	vpunpckhdq xmm1, xmm4, xmm5
	vpunpckhdq ymm6, ymm3, yword [0x7a3]
	vpunpckhdq ymm6, yword [0x7a3]
	vpunpckhdq ymm5, ymm0, ymm2
	vpunpckhdq xmm5, xmm6, xmm4
	vpunpckhdq xmm5, xmm4
	vpunpckhdq xmm3, xmm0, xmm6
	vpunpckhdq ymm1, ymm0, ymm7
	vpunpckhdq ymm1, ymm7
	vpunpckhdq ymm0, ymm2, ymm6

%ifdef ERROR
	vpunpckhdq xmm9, xmm15, xmm13
	vpunpckhdq ymm12, ymm12, ymm9
	vpunpckhdq xmm9, xmm14, xmm8
	vpunpckhdq ymm8, ymm10, ymm12
	vpunpckhdq xmm16, xmm31, xmm27
	vpunpckhdq ymm20, ymm19, ymm17
	vpunpckhdq xmm30, xmm23, xmm31
	vpunpckhdq ymm26, ymm24, ymm18
%endif
