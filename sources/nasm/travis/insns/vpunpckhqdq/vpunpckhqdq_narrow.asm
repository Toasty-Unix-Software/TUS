	vpunpckhqdq xmm5, xmm7, xmm2
	vpunpckhqdq xmm5, xmm2
	vpunpckhqdq xmm5, xmm0, xmm6
	vpunpckhqdq ymm0, ymm6, yword [0x6ad]
	vpunpckhqdq ymm0, yword [0x6ad]
	vpunpckhqdq ymm3, ymm2, yword [0x40b]
	vpunpckhqdq xmm0, xmm7, xmm4
	vpunpckhqdq xmm0, xmm4
	vpunpckhqdq xmm3, xmm7, xmm2
	vpunpckhqdq ymm5, ymm5, yword [0x292]
	vpunpckhqdq ymm5, yword [0x292]
	vpunpckhqdq ymm5, ymm3, ymm0

%ifdef ERROR
	vpunpckhqdq xmm8, xmm10, xmm13
	vpunpckhqdq ymm10, ymm8, ymm14
	vpunpckhqdq xmm10, xmm14, xmm10
	vpunpckhqdq ymm13, ymm15, ymm15
	vpunpckhqdq xmm17, xmm16, xmm23
	vpunpckhqdq ymm17, ymm19, ymm23
	vpunpckhqdq xmm26, xmm21, xmm17
	vpunpckhqdq ymm31, ymm31, ymm21
%endif
