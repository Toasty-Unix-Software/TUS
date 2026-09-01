	vpsubw xmm5, xmm2, xmm7
	vpsubw xmm5, xmm7
	vpsubw xmm5, xmm5, oword [0xbb6]
	vpsubw ymm3, ymm4, yword [0x7a3]
	vpsubw ymm3, yword [0x7a3]
	vpsubw ymm1, ymm5, ymm4
	vpsubw xmm3, xmm1, xmm6
	vpsubw xmm3, xmm6
	vpsubw xmm5, xmm4, xmm5
	vpsubw ymm3, ymm7, ymm1
	vpsubw ymm3, ymm1
	vpsubw ymm1, ymm4, ymm1

%ifdef ERROR
	vpsubw xmm15, xmm11, xmm13
	vpsubw ymm15, ymm9, ymm15
	vpsubw xmm15, xmm13, xmm11
	vpsubw ymm11, ymm15, ymm8
	vpsubw xmm28, xmm30, xmm27
	vpsubw ymm31, ymm16, ymm25
	vpsubw xmm28, xmm29, xmm29
	vpsubw ymm22, ymm30, ymm26
%endif
