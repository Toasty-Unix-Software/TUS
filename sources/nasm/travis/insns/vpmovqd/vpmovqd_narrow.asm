	vpmovqd xmm5, xmm7
	vpmovqd xmm3, xmm0
	vpmovqd xmm6, ymm2
	vpmovqd xmm3, ymm2
	vpmovqd ymm0, zmm4
	vpmovqd ymm2, zmm4
	vpmovqd qword [0x276], xmm5
	vpmovqd qword [0x760], xmm2

%ifdef ERROR
	vpmovqd xmm12, xmm8
	vpmovqd xmm14, ymm15
	vpmovqd ymm10, zmm9
	vpmovqd qword [0x8f4], xmm9
	vpmovqd xmm16, xmm26
	vpmovqd xmm30, ymm24
	vpmovqd ymm18, zmm31
	vpmovqd qword [0xd5b], xmm21
%endif
