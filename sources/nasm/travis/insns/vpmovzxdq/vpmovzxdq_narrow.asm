	vpmovzxdq xmm5, qword [0xac4]
	vpmovzxdq xmm0, xmm7
	vpmovzxdq ymm6, xmm7
	vpmovzxdq ymm5, xmm4
	vpmovzxdq xmm2, xmm1
	vpmovzxdq xmm5, qword [0x665]
	vpmovzxdq ymm5, xmm7
	vpmovzxdq ymm4, oword [0xe48]

%ifdef ERROR
	vpmovzxdq xmm14, xmm13
	vpmovzxdq ymm12, xmm9
	vpmovzxdq xmm13, xmm15
	vpmovzxdq ymm12, xmm9
	vpmovzxdq xmm17, xmm19
	vpmovzxdq ymm27, xmm28
	vpmovzxdq xmm29, xmm27
	vpmovzxdq ymm18, xmm27
%endif
