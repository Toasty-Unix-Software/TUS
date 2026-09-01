default rel
	vpmovqb xmm6, xmm4
	vpmovqb xmm1, xmm2
	vpmovqb xmm2, ymm4
	vpmovqb xmm2, ymm0
	vpmovqb xmm3, zmm0
	vpmovqb xmm0, zmm3
	vpmovqb word [0x52a], xmm0
	vpmovqb word [0xfe7], xmm4
	vpmovqb xmm8, xmm8
	vpmovqb xmm8, ymm11
	vpmovqb xmm13, zmm13
	vpmovqb word [0x541], xmm9
	vpmovqb xmm24, xmm27
	vpmovqb xmm27, ymm23
	vpmovqb xmm18, zmm27
	vpmovqb word [0x836], xmm29
	vpmovqb xmm5{k2}, xmm0
	vpmovqb xmm3{k4}, ymm7
	vpmovqb xmm4{k2}, zmm6
	vpmovqb word [0xfc7]{k4}, xmm5
	vpmovqb dword [0x8e4]{k1}, ymm7
	vpmovqb qword [0xa29]{k5}, zmm0
	vpmovqb word [eax+1], xmm1
	vpmovqb word [eax+64], xmm1
	vpmovqb dword [eax+1], ymm6
	vpmovqb dword [eax+64], ymm6
	vpmovqb qword [eax+1], zmm6
	vpmovqb qword [eax+64], zmm6
	vpmovqb [0xec4], xmm0
	vpmovqb [0x3fe], ymm4
	vpmovqb [0x107], zmm6
