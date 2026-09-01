default rel
	vpmovqd xmm5, xmm7
	vpmovqd xmm3, xmm0
	vpmovqd xmm6, ymm2
	vpmovqd xmm3, ymm2
	vpmovqd ymm0, zmm4
	vpmovqd ymm2, zmm4
	vpmovqd qword [0x276], xmm5
	vpmovqd qword [0x760], xmm2
	vpmovqd xmm12, xmm8
	vpmovqd xmm14, ymm15
	vpmovqd ymm10, zmm9
	vpmovqd qword [0x8f4], xmm9
	vpmovqd xmm16, xmm26
	vpmovqd xmm30, ymm24
	vpmovqd ymm18, zmm31
	vpmovqd qword [0xd5b], xmm21
	vpmovqd xmm4{k5}, xmm0
	vpmovqd xmm6{k2}, ymm4
	vpmovqd ymm6{k2}, zmm1
	vpmovqd qword [0xfdb]{k3}, xmm1
	vpmovqd oword [0x2e7]{k7}, ymm6
	vpmovqd yword [0x39e]{k4}, zmm6
	vpmovqd qword [eax+1], xmm3
	vpmovqd qword [eax+64], xmm6
	vpmovqd oword [eax+1], ymm2
	vpmovqd oword [eax+64], ymm1
	vpmovqd yword [eax+1], zmm0
	vpmovqd yword [eax+64], zmm3
	vpmovqd [0x371], xmm3
	vpmovqd [0xed2], ymm5
	vpmovqd [0x9e9], zmm2
