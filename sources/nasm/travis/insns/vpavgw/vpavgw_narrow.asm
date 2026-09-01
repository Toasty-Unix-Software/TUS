	vpavgw xmm7, xmm7, oword [0xf85]
	vpavgw xmm7, oword [0xf85]
	vpavgw xmm7, xmm5, xmm7
	vpavgw ymm7, ymm0, ymm4
	vpavgw ymm7, ymm4
	vpavgw ymm5, ymm5, ymm4
	vpavgw xmm3, xmm6, xmm0
	vpavgw xmm3, xmm0
	vpavgw xmm7, xmm7, xmm2
	vpavgw ymm1, ymm6, ymm3
	vpavgw ymm1, ymm3
	vpavgw ymm2, ymm5, ymm7

%ifdef ERROR
	vpavgw xmm15, xmm9, xmm9
	vpavgw ymm10, ymm12, ymm14
	vpavgw xmm9, xmm15, xmm10
	vpavgw ymm9, ymm12, ymm9
	vpavgw xmm25, xmm28, xmm31
	vpavgw ymm23, ymm22, ymm22
	vpavgw xmm21, xmm21, xmm18
	vpavgw ymm20, ymm24, ymm27
%endif
