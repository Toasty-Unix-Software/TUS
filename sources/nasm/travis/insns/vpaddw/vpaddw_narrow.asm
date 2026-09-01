	vpaddw xmm5, xmm1, xmm5
	vpaddw xmm5, xmm5
	vpaddw xmm4, xmm4, oword [0xe56]
	vpaddw ymm0, ymm0, ymm5
	vpaddw ymm0, ymm5
	vpaddw ymm4, ymm0, ymm0
	vpaddw xmm5, xmm4, oword [0x9ca]
	vpaddw xmm5, oword [0x9ca]
	vpaddw xmm5, xmm7, xmm7
	vpaddw ymm3, ymm0, ymm4
	vpaddw ymm3, ymm4
	vpaddw ymm1, ymm7, yword [0x3b2]

%ifdef ERROR
	vpaddw xmm11, xmm15, xmm8
	vpaddw ymm14, ymm13, ymm15
	vpaddw xmm14, xmm10, xmm11
	vpaddw ymm10, ymm15, ymm13
	vpaddw xmm23, xmm16, xmm18
	vpaddw ymm22, ymm31, ymm16
	vpaddw xmm30, xmm23, xmm17
	vpaddw ymm31, ymm17, ymm18
%endif
