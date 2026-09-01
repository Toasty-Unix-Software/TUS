	vpmultishiftqb xmm5, xmm7, xmm2
	vpmultishiftqb xmm5, xmm2
	vpmultishiftqb xmm3, xmm5, oword [0x2a6]
	vpmultishiftqb ymm7, ymm6, ymm2
	vpmultishiftqb ymm7, ymm2
	vpmultishiftqb ymm3, ymm0, ymm6
	vpmultishiftqb zmm0, zmm6, zword [0x3b4]
	vpmultishiftqb zmm0, zword [0x3b4]
	vpmultishiftqb zmm1, zmm4, zmm5

%ifdef ERROR
	vpmultishiftqb xmm10, xmm15, xmm10
	vpmultishiftqb ymm11, ymm15, ymm8
	vpmultishiftqb zmm8, zmm12, zmm8
	vpmultishiftqb xmm21, xmm30, xmm18
	vpmultishiftqb ymm19, ymm20, ymm30
	vpmultishiftqb zmm17, zmm21, zmm30
%endif
