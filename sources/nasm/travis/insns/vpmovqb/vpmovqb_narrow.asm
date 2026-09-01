	vpmovqb xmm6, xmm4
	vpmovqb xmm1, xmm2
	vpmovqb xmm2, ymm4
	vpmovqb xmm2, ymm0
	vpmovqb xmm3, zmm0
	vpmovqb xmm0, zmm3
	vpmovqb word [0x52a], xmm0
	vpmovqb word [0xfe7], xmm4

%ifdef ERROR
	vpmovqb xmm8, xmm8
	vpmovqb xmm8, ymm11
	vpmovqb xmm13, zmm13
	vpmovqb word [0x541], xmm9
	vpmovqb xmm24, xmm27
	vpmovqb xmm27, ymm23
	vpmovqb xmm18, zmm27
	vpmovqb word [0x836], xmm29
%endif
