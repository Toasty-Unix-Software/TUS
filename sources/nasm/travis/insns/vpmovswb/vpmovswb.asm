default rel
	vpmovswb xmm3, xmm4
	vpmovswb xmm7, xmm3
	vpmovswb xmm1, ymm1
	vpmovswb xmm1, ymm3
	vpmovswb ymm3, zmm6
	vpmovswb ymm6, zmm7
	vpmovswb qword [0xa00], xmm0
	vpmovswb qword [0x654], xmm1
	vpmovswb xmm13, xmm14
	vpmovswb xmm15, ymm13
	vpmovswb ymm11, zmm8
	vpmovswb qword [0x9a0], xmm8
	vpmovswb xmm20, xmm18
	vpmovswb xmm26, ymm29
	vpmovswb ymm23, zmm28
	vpmovswb qword [0x3fd], xmm30
	vpmovswb xmm7{k1}, xmm0
	vpmovswb xmm5{k6}, ymm7
	vpmovswb ymm3{k6}, zmm1
	vpmovswb qword [0xd4c]{k2}, xmm6
	vpmovswb oword [0x960]{k4}, ymm2
	vpmovswb yword [0xe7d]{k5}, zmm2
	vpmovswb qword [eax+1], xmm5
	vpmovswb qword [eax+64], xmm1
	vpmovswb oword [eax+1], ymm4
	vpmovswb oword [eax+64], ymm6
	vpmovswb yword [eax+1], zmm4
	vpmovswb yword [eax+64], zmm4
	vpmovswb [0xeeb], xmm5
	vpmovswb [0x392], ymm5
	vpmovswb [0x703], zmm6
