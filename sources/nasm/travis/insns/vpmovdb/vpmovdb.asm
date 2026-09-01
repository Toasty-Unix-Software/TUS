default rel
	vpmovdb xmm3, xmm1
	vpmovdb xmm7, xmm5
	vpmovdb xmm1, ymm0
	vpmovdb xmm4, ymm4
	vpmovdb xmm3, zmm0
	vpmovdb xmm1, zmm3
	vpmovdb dword [0x170], xmm0
	vpmovdb dword [0x2b4], xmm0
	vpmovdb xmm13, xmm14
	vpmovdb xmm13, ymm13
	vpmovdb xmm12, zmm11
	vpmovdb dword [0x862], xmm10
	vpmovdb xmm21, xmm26
	vpmovdb xmm17, ymm22
	vpmovdb xmm28, zmm20
	vpmovdb dword [0x649], xmm22
	vpmovdb xmm7{k4}, xmm7
	vpmovdb xmm6{k6}, ymm7
	vpmovdb xmm1{k5}, zmm1
	vpmovdb dword [0x619]{k4}, xmm5
	vpmovdb qword [0xfdb]{k4}, ymm6
	vpmovdb oword [0x1a6]{k5}, zmm2
	vpmovdb dword [eax+1], xmm7
	vpmovdb dword [eax+64], xmm6
	vpmovdb qword [eax+1], ymm2
	vpmovdb qword [eax+64], ymm5
	vpmovdb oword [eax+1], zmm7
	vpmovdb oword [eax+64], zmm3
	vpmovdb [0xb51], xmm7
	vpmovdb [0x2f8], ymm2
	vpmovdb [0x8ea], zmm7
