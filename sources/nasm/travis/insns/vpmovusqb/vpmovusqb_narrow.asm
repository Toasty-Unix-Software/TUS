	vpmovusqb xmm0, xmm0
	vpmovusqb xmm2, xmm3
	vpmovusqb xmm7, ymm4
	vpmovusqb xmm6, ymm0
	vpmovusqb xmm5, zmm2
	vpmovusqb xmm6, zmm1
	vpmovusqb word [0xa4e], xmm7
	vpmovusqb word [0x779], xmm2

%ifdef ERROR
	vpmovusqb xmm13, xmm12
	vpmovusqb xmm11, ymm13
	vpmovusqb xmm13, zmm13
	vpmovusqb word [0x2b8], xmm8
	vpmovusqb xmm27, xmm27
	vpmovusqb xmm23, ymm22
	vpmovusqb xmm21, zmm23
	vpmovusqb word [0xf2b], xmm23
%endif
