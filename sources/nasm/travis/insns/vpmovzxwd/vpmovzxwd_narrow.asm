	vpmovzxwd xmm1, qword [0x314]
	vpmovzxwd xmm4, qword [0xf56]
	vpmovzxwd ymm0, oword [0xe2b]
	vpmovzxwd ymm6, xmm1
	vpmovzxwd xmm7, xmm7
	vpmovzxwd xmm6, qword [0x2ef]
	vpmovzxwd ymm4, oword [0x23d]
	vpmovzxwd ymm3, xmm3

%ifdef ERROR
	vpmovzxwd xmm9, xmm14
	vpmovzxwd ymm14, xmm11
	vpmovzxwd xmm13, xmm14
	vpmovzxwd ymm11, xmm9
	vpmovzxwd xmm24, xmm27
	vpmovzxwd ymm18, xmm18
	vpmovzxwd xmm17, xmm16
	vpmovzxwd ymm26, xmm24
%endif
