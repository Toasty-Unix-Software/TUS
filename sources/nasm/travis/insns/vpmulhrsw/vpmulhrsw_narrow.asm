	vpmulhrsw xmm7, xmm5, oword [0x29f]
	vpmulhrsw xmm7, oword [0x29f]
	vpmulhrsw xmm5, xmm2, xmm7
	vpmulhrsw ymm3, ymm5, ymm7
	vpmulhrsw ymm3, ymm7
	vpmulhrsw ymm4, ymm0, yword [0xfd1]
	vpmulhrsw xmm2, xmm1, xmm0
	vpmulhrsw xmm2, xmm0
	vpmulhrsw xmm3, xmm3, oword [0x79a]
	vpmulhrsw ymm7, ymm1, ymm4
	vpmulhrsw ymm7, ymm4
	vpmulhrsw ymm7, ymm4, ymm0

%ifdef ERROR
	vpmulhrsw xmm8, xmm11, xmm15
	vpmulhrsw ymm10, ymm10, ymm14
	vpmulhrsw xmm13, xmm11, xmm9
	vpmulhrsw ymm13, ymm14, ymm15
	vpmulhrsw xmm31, xmm30, xmm28
	vpmulhrsw ymm25, ymm20, ymm28
	vpmulhrsw xmm21, xmm28, xmm23
	vpmulhrsw ymm24, ymm17, ymm18
%endif
