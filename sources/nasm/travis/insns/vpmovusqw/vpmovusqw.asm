default rel
	vpmovusqw xmm2, xmm4
	vpmovusqw xmm1, xmm4
	vpmovusqw xmm0, ymm1
	vpmovusqw xmm0, ymm1
	vpmovusqw xmm1, zmm7
	vpmovusqw xmm1, zmm7
	vpmovusqw dword [0x89c], xmm4
	vpmovusqw dword [0xd0b], xmm6
	vpmovusqw xmm11, xmm9
	vpmovusqw xmm15, ymm15
	vpmovusqw xmm13, zmm13
	vpmovusqw dword [0xfdd], xmm15
	vpmovusqw xmm22, xmm18
	vpmovusqw xmm22, ymm22
	vpmovusqw xmm21, zmm30
	vpmovusqw dword [0xced], xmm30
	vpmovusqw xmm1{k3}, xmm4
	vpmovusqw xmm5{k2}, ymm4
	vpmovusqw xmm2{k2}, zmm0
	vpmovusqw dword [0xf02]{k3}, xmm6
	vpmovusqw qword [0x419]{k3}, ymm1
	vpmovusqw oword [0x4ba]{k1}, zmm2
	vpmovusqw dword [eax+1], xmm1
	vpmovusqw dword [eax+64], xmm2
	vpmovusqw qword [eax+1], ymm0
	vpmovusqw qword [eax+64], ymm5
	vpmovusqw oword [eax+1], zmm7
	vpmovusqw oword [eax+64], zmm7
	vpmovusqw [0x27d], xmm2
	vpmovusqw [0x6ff], ymm7
	vpmovusqw [0xa0e], zmm7
