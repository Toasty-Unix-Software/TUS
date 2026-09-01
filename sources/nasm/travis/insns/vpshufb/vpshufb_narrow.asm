	vpshufb xmm4, xmm4, xmm3
	vpshufb xmm4, xmm3
	vpshufb xmm3, xmm0, xmm4
	vpshufb ymm3, ymm3, ymm0
	vpshufb ymm3, ymm0
	vpshufb ymm0, ymm0, yword [0xee1]
	vpshufb xmm0, xmm2, oword [0xf56]
	vpshufb xmm0, oword [0xf56]
	vpshufb xmm3, xmm4, xmm1
	vpshufb ymm1, ymm2, yword [0x2bf]
	vpshufb ymm1, yword [0x2bf]
	vpshufb ymm2, ymm7, ymm4

%ifdef ERROR
	vpshufb xmm15, xmm10, xmm10
	vpshufb ymm8, ymm13, ymm12
	vpshufb xmm10, xmm12, xmm15
	vpshufb ymm10, ymm12, ymm15
	vpshufb xmm21, xmm18, xmm22
	vpshufb ymm29, ymm31, ymm26
	vpshufb xmm24, xmm19, xmm24
	vpshufb ymm23, ymm31, ymm31
%endif
