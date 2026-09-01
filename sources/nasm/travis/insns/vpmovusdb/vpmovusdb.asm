default rel
	vpmovusdb xmm1, xmm0
	vpmovusdb xmm0, xmm1
	vpmovusdb xmm1, ymm0
	vpmovusdb xmm5, ymm2
	vpmovusdb xmm1, zmm0
	vpmovusdb xmm1, zmm5
	vpmovusdb dword [0xaa9], xmm7
	vpmovusdb dword [0xe90], xmm1
	vpmovusdb xmm8, xmm9
	vpmovusdb xmm14, ymm12
	vpmovusdb xmm12, zmm14
	vpmovusdb dword [0x653], xmm11
	vpmovusdb xmm24, xmm30
	vpmovusdb xmm17, ymm22
	vpmovusdb xmm22, zmm25
	vpmovusdb dword [0x1ec], xmm27
	vpmovusdb xmm3{k1}, xmm1
	vpmovusdb xmm3{k2}, ymm1
	vpmovusdb xmm1{k1}, zmm4
	vpmovusdb dword [0xdd3]{k7}, xmm1
	vpmovusdb qword [0x2f8]{k4}, ymm6
	vpmovusdb oword [0x467]{k7}, zmm3
	vpmovusdb dword [eax+1], xmm6
	vpmovusdb dword [eax+64], xmm3
	vpmovusdb qword [eax+1], ymm2
	vpmovusdb qword [eax+64], ymm5
	vpmovusdb oword [eax+1], zmm5
	vpmovusdb oword [eax+64], zmm0
	vpmovusdb [0x484], xmm4
	vpmovusdb [0x6db], ymm7
	vpmovusdb [0x251], zmm0
