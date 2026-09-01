default rel
	vpmovusqd xmm6, xmm1
	vpmovusqd xmm2, xmm4
	vpmovusqd xmm5, ymm4
	vpmovusqd xmm7, ymm5
	vpmovusqd ymm2, zmm0
	vpmovusqd ymm7, zmm7
	vpmovusqd qword [0xb22], xmm3
	vpmovusqd qword [0x6c7], xmm5
	vpmovusqd xmm15, xmm11
	vpmovusqd xmm8, ymm15
	vpmovusqd ymm12, zmm8
	vpmovusqd qword [0x8af], xmm9
	vpmovusqd xmm29, xmm29
	vpmovusqd xmm30, ymm17
	vpmovusqd ymm31, zmm20
	vpmovusqd qword [0x67e], xmm16
	vpmovusqd xmm3{k5}, xmm2
	vpmovusqd xmm1{k1}, ymm5
	vpmovusqd ymm0{k5}, zmm3
	vpmovusqd qword [0x357]{k4}, xmm6
	vpmovusqd oword [0xc68]{k7}, ymm4
	vpmovusqd yword [0x9b1]{k6}, zmm5
	vpmovusqd qword [eax+1], xmm6
	vpmovusqd qword [eax+64], xmm5
	vpmovusqd oword [eax+1], ymm4
	vpmovusqd oword [eax+64], ymm2
	vpmovusqd yword [eax+1], zmm1
	vpmovusqd yword [eax+64], zmm0
	vpmovusqd [0xd21], xmm7
	vpmovusqd [0x44c], ymm3
	vpmovusqd [0x1d2], zmm7
