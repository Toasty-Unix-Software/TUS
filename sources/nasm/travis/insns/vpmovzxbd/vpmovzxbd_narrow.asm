	vpmovzxbd xmm7, xmm1
	vpmovzxbd xmm6, xmm2
	vpmovzxbd ymm2, qword [0x810]
	vpmovzxbd ymm0, qword [0x62e]
	vpmovzxbd ymm4, xmm5
	vpmovzxbd ymm7, xmm3
	vpmovzxbd xmm2, xmm6
	vpmovzxbd xmm4, xmm3

%ifdef ERROR
	vpmovzxbd xmm15, xmm10
	vpmovzxbd ymm11, qword [0xa16]
	vpmovzxbd ymm13, xmm15
	vpmovzxbd xmm13, xmm8
	vpmovzxbd xmm26, xmm17
	vpmovzxbd ymm24, qword [0x632]
	vpmovzxbd ymm18, xmm30
	vpmovzxbd xmm19, xmm21
%endif
