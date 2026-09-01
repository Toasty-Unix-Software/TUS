	vpsubusw xmm5, xmm0, xmm5
	vpsubusw xmm5, xmm5
	vpsubusw xmm3, xmm2, xmm3
	vpsubusw ymm4, ymm6, ymm1
	vpsubusw ymm4, ymm1
	vpsubusw ymm1, ymm3, ymm5
	vpsubusw xmm4, xmm2, xmm7
	vpsubusw xmm4, xmm7
	vpsubusw xmm3, xmm6, xmm3
	vpsubusw ymm6, ymm0, yword [0xa24]
	vpsubusw ymm6, yword [0xa24]
	vpsubusw ymm2, ymm0, yword [0xf54]

%ifdef ERROR
	vpsubusw xmm12, xmm13, xmm13
	vpsubusw ymm12, ymm14, ymm11
	vpsubusw xmm9, xmm9, xmm8
	vpsubusw ymm12, ymm12, ymm13
	vpsubusw xmm21, xmm22, xmm29
	vpsubusw ymm27, ymm31, ymm29
	vpsubusw xmm24, xmm16, xmm28
	vpsubusw ymm23, ymm30, ymm30
%endif
