	vpmaxuw xmm1, xmm0, xmm3
	vpmaxuw xmm1, xmm3
	vpmaxuw xmm6, xmm0, oword [0xbfa]
	vpmaxuw ymm3, ymm6, ymm7
	vpmaxuw ymm3, ymm7
	vpmaxuw ymm0, ymm3, yword [0x37f]
	vpmaxuw xmm3, xmm3, xmm7
	vpmaxuw xmm3, xmm7
	vpmaxuw xmm2, xmm5, xmm1
	vpmaxuw ymm0, ymm6, ymm6
	vpmaxuw ymm0, ymm6
	vpmaxuw ymm3, ymm7, ymm3

%ifdef ERROR
	vpmaxuw xmm11, xmm10, xmm15
	vpmaxuw ymm15, ymm12, ymm10
	vpmaxuw xmm12, xmm15, xmm12
	vpmaxuw ymm15, ymm9, ymm15
	vpmaxuw xmm19, xmm27, xmm24
	vpmaxuw ymm26, ymm18, ymm28
	vpmaxuw xmm17, xmm21, xmm25
	vpmaxuw ymm27, ymm24, ymm26
%endif
