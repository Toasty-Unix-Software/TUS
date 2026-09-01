	vpshldvw xmm6, xmm6, oword [0xc56]
	vpshldvw xmm6, oword [0xc56]
	vpshldvw xmm5, xmm6, oword [0xd83]
	vpshldvw ymm0, ymm2, ymm7
	vpshldvw ymm0, ymm7
	vpshldvw ymm1, ymm6, yword [0xae8]
	vpshldvw zmm2, zmm0, zword [0x186]
	vpshldvw zmm2, zword [0x186]
	vpshldvw zmm0, zmm7, zword [0x590]

%ifdef ERROR
	vpshldvw xmm11, xmm14, xmm14
	vpshldvw ymm14, ymm13, ymm9
	vpshldvw zmm8, zmm15, zmm14
	vpshldvw xmm17, xmm25, xmm29
	vpshldvw ymm18, ymm24, ymm27
	vpshldvw zmm31, zmm25, zmm31
%endif
