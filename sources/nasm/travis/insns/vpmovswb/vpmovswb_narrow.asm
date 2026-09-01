	vpmovswb xmm3, xmm4
	vpmovswb xmm7, xmm3
	vpmovswb xmm1, ymm1
	vpmovswb xmm1, ymm3
	vpmovswb ymm3, zmm6
	vpmovswb ymm6, zmm7
	vpmovswb qword [0xa00], xmm0
	vpmovswb qword [0x654], xmm1

%ifdef ERROR
	vpmovswb xmm13, xmm14
	vpmovswb xmm15, ymm13
	vpmovswb ymm11, zmm8
	vpmovswb qword [0x9a0], xmm8
	vpmovswb xmm20, xmm18
	vpmovswb xmm26, ymm29
	vpmovswb ymm23, zmm28
	vpmovswb qword [0x3fd], xmm30
%endif
