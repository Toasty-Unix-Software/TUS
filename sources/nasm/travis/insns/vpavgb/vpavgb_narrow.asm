	vpavgb xmm7, xmm2, xmm6
	vpavgb xmm7, xmm6
	vpavgb xmm2, xmm6, xmm6
	vpavgb ymm4, ymm3, ymm7
	vpavgb ymm4, ymm7
	vpavgb ymm2, ymm5, ymm2
	vpavgb xmm4, xmm4, xmm1
	vpavgb xmm4, xmm1
	vpavgb xmm6, xmm5, xmm2
	vpavgb ymm1, ymm4, ymm1
	vpavgb ymm1, ymm1
	vpavgb ymm2, ymm1, ymm0

%ifdef ERROR
	vpavgb xmm9, xmm8, xmm14
	vpavgb ymm10, ymm14, ymm14
	vpavgb xmm8, xmm15, xmm10
	vpavgb ymm9, ymm10, ymm15
	vpavgb xmm30, xmm26, xmm17
	vpavgb ymm22, ymm17, ymm25
	vpavgb xmm23, xmm18, xmm26
	vpavgb ymm27, ymm19, ymm25
%endif
