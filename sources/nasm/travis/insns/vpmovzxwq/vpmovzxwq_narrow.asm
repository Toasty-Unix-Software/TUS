	vpmovzxwq xmm5, dword [0x6e9]
	vpmovzxwq xmm2, dword [0x753]
	vpmovzxwq ymm7, qword [0x34f]
	vpmovzxwq ymm2, qword [0x807]
	vpmovzxwq ymm5, xmm4
	vpmovzxwq ymm0, xmm5
	vpmovzxwq xmm1, dword [0xf62]
	vpmovzxwq xmm7, xmm5

%ifdef ERROR
	vpmovzxwq xmm11, xmm12
	vpmovzxwq ymm10, qword [0x7fc]
	vpmovzxwq ymm10, xmm9
	vpmovzxwq xmm9, xmm15
	vpmovzxwq xmm21, xmm30
	vpmovzxwq ymm18, qword [0xec9]
	vpmovzxwq ymm20, xmm16
	vpmovzxwq xmm26, xmm29
%endif
