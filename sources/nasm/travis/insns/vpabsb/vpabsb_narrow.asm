	vpabsb xmm1, xmm0
	vpabsb xmm3, oword [0x4a8]
	vpabsb ymm0, ymm1
	vpabsb ymm3, ymm5
	vpabsb xmm7, xmm2
	vpabsb xmm2, xmm7
	vpabsb ymm7, ymm4
	vpabsb ymm2, ymm5

%ifdef ERROR
	vpabsb xmm14, xmm15
	vpabsb ymm9, ymm14
	vpabsb xmm9, xmm9
	vpabsb ymm13, ymm8
	vpabsb xmm26, xmm17
	vpabsb ymm18, ymm25
	vpabsb xmm20, xmm19
	vpabsb ymm20, ymm19
%endif
