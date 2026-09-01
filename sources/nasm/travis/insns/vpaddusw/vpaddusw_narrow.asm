	vpaddusw xmm6, xmm2, oword [0x490]
	vpaddusw xmm6, oword [0x490]
	vpaddusw xmm0, xmm3, xmm0
	vpaddusw ymm7, ymm6, ymm7
	vpaddusw ymm7, ymm7
	vpaddusw ymm3, ymm5, yword [0x961]
	vpaddusw xmm3, xmm4, oword [0xfac]
	vpaddusw xmm3, oword [0xfac]
	vpaddusw xmm4, xmm3, oword [0x89c]
	vpaddusw ymm0, ymm6, yword [0x7cc]
	vpaddusw ymm0, yword [0x7cc]
	vpaddusw ymm6, ymm7, ymm4

%ifdef ERROR
	vpaddusw xmm15, xmm14, xmm12
	vpaddusw ymm12, ymm9, ymm9
	vpaddusw xmm15, xmm12, xmm15
	vpaddusw ymm15, ymm10, ymm8
	vpaddusw xmm29, xmm19, xmm17
	vpaddusw ymm30, ymm19, ymm31
	vpaddusw xmm21, xmm26, xmm24
	vpaddusw ymm28, ymm25, ymm26
%endif
