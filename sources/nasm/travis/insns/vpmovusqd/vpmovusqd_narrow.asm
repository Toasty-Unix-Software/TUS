	vpmovusqd xmm6, xmm1
	vpmovusqd xmm2, xmm4
	vpmovusqd xmm5, ymm4
	vpmovusqd xmm7, ymm5
	vpmovusqd ymm2, zmm0
	vpmovusqd ymm7, zmm7
	vpmovusqd qword [0xb22], xmm3
	vpmovusqd qword [0x6c7], xmm5

%ifdef ERROR
	vpmovusqd xmm15, xmm11
	vpmovusqd xmm8, ymm15
	vpmovusqd ymm12, zmm8
	vpmovusqd qword [0x8af], xmm9
	vpmovusqd xmm29, xmm29
	vpmovusqd xmm30, ymm17
	vpmovusqd ymm31, zmm20
	vpmovusqd qword [0x67e], xmm16
%endif
