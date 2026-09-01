default rel
	vpmovwb xmm6, xmm7
	vpmovwb xmm4, xmm4
	vpmovwb xmm3, ymm2
	vpmovwb xmm4, ymm2
	vpmovwb ymm1, zmm4
	vpmovwb ymm7, zmm7
	vpmovwb qword [0xf1b], xmm3
	vpmovwb qword [0x6d8], xmm1
	vpmovwb xmm15, xmm11
	vpmovwb xmm12, ymm10
	vpmovwb ymm9, zmm11
	vpmovwb qword [0x63b], xmm10
	vpmovwb xmm30, xmm26
	vpmovwb xmm29, ymm25
	vpmovwb ymm20, zmm19
	vpmovwb qword [0xb84], xmm18
	vpmovwb xmm3{k3}, xmm3
	vpmovwb xmm6{k1}, ymm4
	vpmovwb ymm4{k5}, zmm5
	vpmovwb qword [0x3fb]{k7}, xmm6
	vpmovwb oword [0x898]{k6}, ymm7
	vpmovwb yword [0xe37]{k6}, zmm0
	vpmovwb qword [eax+1], xmm0
	vpmovwb qword [eax+64], xmm7
	vpmovwb oword [eax+1], ymm4
	vpmovwb oword [eax+64], ymm5
	vpmovwb yword [eax+1], zmm4
	vpmovwb yword [eax+64], zmm0
	vpmovwb [0x583], xmm7
	vpmovwb [0x6f0], ymm1
	vpmovwb [0x804], zmm0
