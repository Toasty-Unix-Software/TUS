	vpsubd xmm4, xmm2, oword [0xbf2]
	vpsubd xmm4, oword [0xbf2]
	vpsubd xmm7, xmm1, xmm5
	vpsubd ymm5, ymm4, yword [0xa69]
	vpsubd ymm5, yword [0xa69]
	vpsubd ymm1, ymm1, ymm0
	vpsubd xmm2, xmm2, xmm0
	vpsubd xmm2, xmm0
	vpsubd xmm0, xmm0, xmm3
	vpsubd ymm6, ymm7, yword [0xf5d]
	vpsubd ymm6, yword [0xf5d]
	vpsubd ymm0, ymm0, yword [0x56e]

%ifdef ERROR
	vpsubd xmm10, xmm12, xmm12
	vpsubd ymm12, ymm14, ymm8
	vpsubd xmm9, xmm15, xmm8
	vpsubd ymm12, ymm12, ymm15
	vpsubd xmm23, xmm24, xmm30
	vpsubd ymm17, ymm16, ymm29
	vpsubd xmm19, xmm25, xmm23
	vpsubd ymm31, ymm27, ymm27
%endif
