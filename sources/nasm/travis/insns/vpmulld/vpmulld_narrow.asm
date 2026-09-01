	vpmulld xmm2, xmm6, xmm0
	vpmulld xmm2, xmm0
	vpmulld xmm7, xmm6, oword [0x3ea]
	vpmulld ymm6, ymm7, ymm1
	vpmulld ymm6, ymm1
	vpmulld ymm2, ymm5, ymm1
	vpmulld xmm3, xmm3, xmm4
	vpmulld xmm3, xmm4
	vpmulld xmm3, xmm1, oword [0xb78]
	vpmulld ymm0, ymm1, ymm0
	vpmulld ymm0, ymm0
	vpmulld ymm2, ymm2, ymm5

%ifdef ERROR
	vpmulld xmm11, xmm11, xmm8
	vpmulld ymm11, ymm10, ymm14
	vpmulld xmm15, xmm9, xmm12
	vpmulld ymm9, ymm15, ymm8
	vpmulld xmm20, xmm18, xmm17
	vpmulld ymm20, ymm24, ymm24
	vpmulld xmm22, xmm18, xmm26
	vpmulld ymm23, ymm22, ymm17
%endif
