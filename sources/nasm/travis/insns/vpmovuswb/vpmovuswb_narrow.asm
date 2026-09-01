	vpmovuswb xmm0, xmm5
	vpmovuswb xmm4, xmm2
	vpmovuswb xmm3, ymm1
	vpmovuswb xmm7, ymm4
	vpmovuswb ymm3, zmm6
	vpmovuswb ymm0, zmm3
	vpmovuswb qword [0x309], xmm2
	vpmovuswb qword [0xb30], xmm1

%ifdef ERROR
	vpmovuswb xmm12, xmm11
	vpmovuswb xmm14, ymm11
	vpmovuswb ymm15, zmm8
	vpmovuswb qword [0xe0e], xmm10
	vpmovuswb xmm16, xmm30
	vpmovuswb xmm20, ymm23
	vpmovuswb ymm17, zmm27
	vpmovuswb qword [0xd75], xmm18
%endif
