	vpmullw xmm1, xmm7, xmm5
	vpmullw xmm1, xmm5
	vpmullw xmm2, xmm0, xmm7
	vpmullw ymm5, ymm5, ymm4
	vpmullw ymm5, ymm4
	vpmullw ymm4, ymm1, ymm7
	vpmullw xmm5, xmm7, xmm6
	vpmullw xmm5, xmm6
	vpmullw xmm5, xmm5, xmm1
	vpmullw ymm0, ymm6, yword [0x6d2]
	vpmullw ymm0, yword [0x6d2]
	vpmullw ymm7, ymm3, ymm7

%ifdef ERROR
	vpmullw xmm10, xmm14, xmm8
	vpmullw ymm10, ymm15, ymm11
	vpmullw xmm14, xmm9, xmm10
	vpmullw ymm15, ymm14, ymm10
	vpmullw xmm20, xmm18, xmm18
	vpmullw ymm25, ymm20, ymm25
	vpmullw xmm25, xmm26, xmm29
	vpmullw ymm21, ymm18, ymm23
%endif
