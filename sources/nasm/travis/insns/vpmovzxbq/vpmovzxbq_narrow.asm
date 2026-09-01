	vpmovzxbq xmm1, word [0x166]
	vpmovzxbq xmm1, word [0x2ba]
	vpmovzxbq ymm5, dword [0x405]
	vpmovzxbq ymm3, dword [0x4ad]
	vpmovzxbq ymm6, xmm2
	vpmovzxbq ymm2, xmm5
	vpmovzxbq xmm6, xmm3
	vpmovzxbq xmm0, xmm5

%ifdef ERROR
	vpmovzxbq xmm11, xmm14
	vpmovzxbq ymm14, dword [0x2ec]
	vpmovzxbq ymm14, xmm12
	vpmovzxbq xmm11, xmm12
	vpmovzxbq xmm27, xmm29
	vpmovzxbq ymm27, dword [0x2ed]
	vpmovzxbq ymm26, xmm20
	vpmovzxbq xmm25, xmm25
%endif
