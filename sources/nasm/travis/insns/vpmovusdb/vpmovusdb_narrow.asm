	vpmovusdb xmm1, xmm0
	vpmovusdb xmm0, xmm1
	vpmovusdb xmm1, ymm0
	vpmovusdb xmm5, ymm2
	vpmovusdb xmm1, zmm0
	vpmovusdb xmm1, zmm5
	vpmovusdb dword [0xaa9], xmm7
	vpmovusdb dword [0xe90], xmm1

%ifdef ERROR
	vpmovusdb xmm8, xmm9
	vpmovusdb xmm14, ymm12
	vpmovusdb xmm12, zmm14
	vpmovusdb dword [0x653], xmm11
	vpmovusdb xmm24, xmm30
	vpmovusdb xmm17, ymm22
	vpmovusdb xmm22, zmm25
	vpmovusdb dword [0x1ec], xmm27
%endif
