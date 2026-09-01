	vpmovdb xmm3, xmm1
	vpmovdb xmm7, xmm5
	vpmovdb xmm1, ymm0
	vpmovdb xmm4, ymm4
	vpmovdb xmm3, zmm0
	vpmovdb xmm1, zmm3
	vpmovdb dword [0x170], xmm0
	vpmovdb dword [0x2b4], xmm0

%ifdef ERROR
	vpmovdb xmm13, xmm14
	vpmovdb xmm13, ymm13
	vpmovdb xmm12, zmm11
	vpmovdb dword [0x862], xmm10
	vpmovdb xmm21, xmm26
	vpmovdb xmm17, ymm22
	vpmovdb xmm28, zmm20
	vpmovdb dword [0x649], xmm22
%endif
