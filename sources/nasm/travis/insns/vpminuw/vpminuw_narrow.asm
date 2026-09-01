	vpminuw xmm7, xmm3, oword [0xeba]
	vpminuw xmm7, oword [0xeba]
	vpminuw xmm1, xmm4, xmm1
	vpminuw ymm2, ymm4, ymm1
	vpminuw ymm2, ymm1
	vpminuw ymm3, ymm7, ymm7
	vpminuw xmm5, xmm1, xmm4
	vpminuw xmm5, xmm4
	vpminuw xmm0, xmm1, xmm0
	vpminuw ymm7, ymm3, ymm6
	vpminuw ymm7, ymm6
	vpminuw ymm1, ymm4, yword [0x459]

%ifdef ERROR
	vpminuw xmm13, xmm9, xmm13
	vpminuw ymm10, ymm15, ymm14
	vpminuw xmm14, xmm8, xmm14
	vpminuw ymm9, ymm9, ymm12
	vpminuw xmm23, xmm31, xmm21
	vpminuw ymm20, ymm24, ymm26
	vpminuw xmm16, xmm31, xmm17
	vpminuw ymm24, ymm28, ymm30
%endif
