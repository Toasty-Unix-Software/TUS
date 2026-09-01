default rel
	vpmovusqb xmm0, xmm0
	vpmovusqb xmm2, xmm3
	vpmovusqb xmm7, ymm4
	vpmovusqb xmm6, ymm0
	vpmovusqb xmm5, zmm2
	vpmovusqb xmm6, zmm1
	vpmovusqb word [0xa4e], xmm7
	vpmovusqb word [0x779], xmm2
	vpmovusqb xmm13, xmm12
	vpmovusqb xmm11, ymm13
	vpmovusqb xmm13, zmm13
	vpmovusqb word [0x2b8], xmm8
	vpmovusqb xmm27, xmm27
	vpmovusqb xmm23, ymm22
	vpmovusqb xmm21, zmm23
	vpmovusqb word [0xf2b], xmm23
	vpmovusqb xmm3{k2}, xmm3
	vpmovusqb xmm5{k4}, ymm6
	vpmovusqb xmm3{k3}, zmm5
	vpmovusqb word [0x8ab]{k3}, xmm3
	vpmovusqb dword [0x811]{k7}, ymm7
	vpmovusqb qword [0xe9c]{k3}, zmm7
	vpmovusqb word [eax+1], xmm1
	vpmovusqb word [eax+64], xmm6
	vpmovusqb dword [eax+1], ymm5
	vpmovusqb dword [eax+64], ymm1
	vpmovusqb qword [eax+1], zmm1
	vpmovusqb qword [eax+64], zmm3
	vpmovusqb [0x4c3], xmm4
	vpmovusqb [0xd0e], ymm0
	vpmovusqb [0xca3], zmm7
