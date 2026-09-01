default rel
	vpmovuswb xmm0, xmm5
	vpmovuswb xmm4, xmm2
	vpmovuswb xmm3, ymm1
	vpmovuswb xmm7, ymm4
	vpmovuswb ymm3, zmm6
	vpmovuswb ymm0, zmm3
	vpmovuswb qword [0x309], xmm2
	vpmovuswb qword [0xb30], xmm1
	vpmovuswb xmm12, xmm11
	vpmovuswb xmm14, ymm11
	vpmovuswb ymm15, zmm8
	vpmovuswb qword [0xe0e], xmm10
	vpmovuswb xmm16, xmm30
	vpmovuswb xmm20, ymm23
	vpmovuswb ymm17, zmm27
	vpmovuswb qword [0xd75], xmm18
	vpmovuswb xmm2{k4}, xmm7
	vpmovuswb xmm7{k1}, ymm7
	vpmovuswb ymm0{k7}, zmm2
	vpmovuswb qword [0xa0f]{k3}, xmm2
	vpmovuswb oword [0xbc3]{k3}, ymm0
	vpmovuswb yword [0x726]{k7}, zmm5
	vpmovuswb qword [eax+1], xmm0
	vpmovuswb qword [eax+64], xmm1
	vpmovuswb oword [eax+1], ymm7
	vpmovuswb oword [eax+64], ymm5
	vpmovuswb yword [eax+1], zmm1
	vpmovuswb yword [eax+64], zmm6
	vpmovuswb [0xcfc], xmm0
	vpmovuswb [0x57d], ymm6
	vpmovuswb [0x551], zmm1
