	vpsubsb xmm3, xmm1, xmm1
	vpsubsb xmm3, xmm1
	vpsubsb xmm7, xmm3, xmm1
	vpsubsb ymm6, ymm1, ymm5
	vpsubsb ymm6, ymm5
	vpsubsb ymm0, ymm6, yword [0xa54]
	vpsubsb xmm3, xmm7, xmm2
	vpsubsb xmm3, xmm2
	vpsubsb xmm3, xmm6, xmm1
	vpsubsb ymm7, ymm4, ymm0
	vpsubsb ymm7, ymm0
	vpsubsb ymm4, ymm5, ymm2

%ifdef ERROR
	vpsubsb xmm10, xmm11, xmm15
	vpsubsb ymm15, ymm12, ymm12
	vpsubsb xmm9, xmm9, xmm8
	vpsubsb ymm15, ymm12, ymm8
	vpsubsb xmm28, xmm26, xmm30
	vpsubsb ymm30, ymm16, ymm30
	vpsubsb xmm25, xmm19, xmm17
	vpsubsb ymm17, ymm23, ymm25
%endif
