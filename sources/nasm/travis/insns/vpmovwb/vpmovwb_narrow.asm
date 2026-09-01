	vpmovwb xmm6, xmm7
	vpmovwb xmm4, xmm4
	vpmovwb xmm3, ymm2
	vpmovwb xmm4, ymm2
	vpmovwb ymm1, zmm4
	vpmovwb ymm7, zmm7
	vpmovwb qword [0xf1b], xmm3
	vpmovwb qword [0x6d8], xmm1

%ifdef ERROR
	vpmovwb xmm15, xmm11
	vpmovwb xmm12, ymm10
	vpmovwb ymm9, zmm11
	vpmovwb qword [0x63b], xmm10
	vpmovwb xmm30, xmm26
	vpmovwb xmm29, ymm25
	vpmovwb ymm20, zmm19
	vpmovwb qword [0xb84], xmm18
%endif
