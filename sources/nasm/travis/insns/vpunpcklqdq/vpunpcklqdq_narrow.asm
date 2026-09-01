	vpunpcklqdq xmm0, xmm0, xmm6
	vpunpcklqdq xmm0, xmm6
	vpunpcklqdq xmm6, xmm7, xmm7
	vpunpcklqdq ymm7, ymm4, ymm4
	vpunpcklqdq ymm7, ymm4
	vpunpcklqdq ymm5, ymm6, ymm1
	vpunpcklqdq xmm7, xmm7, xmm2
	vpunpcklqdq xmm7, xmm2
	vpunpcklqdq xmm1, xmm2, xmm0
	vpunpcklqdq ymm1, ymm1, ymm7
	vpunpcklqdq ymm1, ymm7
	vpunpcklqdq ymm5, ymm7, ymm7

%ifdef ERROR
	vpunpcklqdq xmm12, xmm10, xmm10
	vpunpcklqdq ymm15, ymm11, ymm9
	vpunpcklqdq xmm14, xmm14, xmm13
	vpunpcklqdq ymm9, ymm11, ymm9
	vpunpcklqdq xmm22, xmm31, xmm20
	vpunpcklqdq ymm24, ymm20, ymm24
	vpunpcklqdq xmm25, xmm17, xmm24
	vpunpcklqdq ymm18, ymm17, ymm20
%endif
