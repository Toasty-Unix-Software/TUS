	vpminud xmm0, xmm0, xmm1
	vpminud xmm0, xmm1
	vpminud xmm7, xmm1, xmm3
	vpminud ymm2, ymm4, ymm3
	vpminud ymm2, ymm3
	vpminud ymm1, ymm7, ymm4
	vpminud xmm0, xmm5, oword [0x7d7]
	vpminud xmm0, oword [0x7d7]
	vpminud xmm3, xmm0, xmm3
	vpminud ymm2, ymm6, yword [0x83e]
	vpminud ymm2, yword [0x83e]
	vpminud ymm2, ymm6, ymm0

%ifdef ERROR
	vpminud xmm9, xmm10, xmm9
	vpminud ymm13, ymm12, ymm12
	vpminud xmm15, xmm14, xmm9
	vpminud ymm11, ymm10, ymm15
	vpminud xmm22, xmm31, xmm23
	vpminud ymm26, ymm25, ymm31
	vpminud xmm25, xmm25, xmm27
	vpminud ymm29, ymm31, ymm24
%endif
