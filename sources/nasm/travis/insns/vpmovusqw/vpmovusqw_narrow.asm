	vpmovusqw xmm2, xmm4
	vpmovusqw xmm1, xmm4
	vpmovusqw xmm0, ymm1
	vpmovusqw xmm0, ymm1
	vpmovusqw xmm1, zmm7
	vpmovusqw xmm1, zmm7
	vpmovusqw dword [0x89c], xmm4
	vpmovusqw dword [0xd0b], xmm6

%ifdef ERROR
	vpmovusqw xmm11, xmm9
	vpmovusqw xmm15, ymm15
	vpmovusqw xmm13, zmm13
	vpmovusqw dword [0xfdd], xmm15
	vpmovusqw xmm22, xmm18
	vpmovusqw xmm22, ymm22
	vpmovusqw xmm21, zmm30
	vpmovusqw dword [0xced], xmm30
%endif
