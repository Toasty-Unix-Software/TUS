default rel
	vinsertf64x4 zmm4, zmm2, ymm2, 0x6a
	vinsertf64x4 zmm4, ymm2, 0x6a
	vinsertf64x4 zmm6, zmm4, ymm7, 0x23
	vinsertf64x4 zmm14, zmm13, ymm11, 0x8f
	vinsertf64x4 zmm27, zmm18, ymm24, 0x68
	vinsertf64x4 zmm7{k4}, zmm1, ymm5, 0xc0
	vinsertf64x4 zmm2{k7}{z}, zmm6, yword [0xe41], 0x81
	vinsertf64x4 zmm6, zmm1, yword [eax+1], 0x47
	vinsertf64x4 zmm3, zmm1, yword [eax+64], 0x4b
	vinsertf64x4 zmm3, zmm7, [0x8e0], 0xd5
