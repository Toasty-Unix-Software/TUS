default rel
	vpmultishiftqb xmm5, xmm7, xmm2
	vpmultishiftqb xmm5, xmm2
	vpmultishiftqb xmm3, xmm5, oword [0x2a6]
	vpmultishiftqb ymm7, ymm6, ymm2
	vpmultishiftqb ymm7, ymm2
	vpmultishiftqb ymm3, ymm0, ymm6
	vpmultishiftqb zmm0, zmm6, zword [0x3b4]
	vpmultishiftqb zmm0, zword [0x3b4]
	vpmultishiftqb zmm1, zmm4, zmm5
	vpmultishiftqb xmm10, xmm15, xmm10
	vpmultishiftqb ymm11, ymm15, ymm8
	vpmultishiftqb zmm8, zmm12, zmm8
	vpmultishiftqb xmm21, xmm30, xmm18
	vpmultishiftqb ymm19, ymm20, ymm30
	vpmultishiftqb zmm17, zmm21, zmm30
	vpmultishiftqb xmm3{k6}, xmm7, xmm3
	vpmultishiftqb ymm7{k4}, ymm2, ymm6
	vpmultishiftqb zmm2{k4}, zmm6, zmm7
	vpmultishiftqb xmm2{k7}{z}, xmm3, xmm7
	vpmultishiftqb ymm1{k4}{z}, ymm2, ymm1
	vpmultishiftqb zmm6{k5}{z}, zmm5, zmm2
	vpmultishiftqb xmm7, xmm4, oword [eax+1]
	vpmultishiftqb xmm0, xmm4, oword [eax+64]
	vpmultishiftqb ymm6, ymm1, yword [eax+1]
	vpmultishiftqb ymm2, ymm5, yword [eax+64]
	vpmultishiftqb zmm7, zmm5, zword [eax+1]
	vpmultishiftqb zmm5, zmm3, zword [eax+64]
	vpmultishiftqb xmm0, xmm5, [0xb2d]
	vpmultishiftqb ymm3, ymm5, [0x3fa]
	vpmultishiftqb zmm3, zmm3, [0x2e4]
