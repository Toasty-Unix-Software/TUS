default rel
	vinserti64x2 ymm5, ymm2, xmm0, 0xe9
	vinserti64x2 ymm5, xmm0, 0xe9
	vinserti64x2 ymm7, ymm4, xmm1, 0xdc
	vinserti64x2 zmm7, zmm5, xmm6, 0x9b
	vinserti64x2 zmm7, xmm6, 0x9b
	vinserti64x2 zmm0, zmm6, xmm6, 0x60
	vinserti64x2 ymm15, ymm13, xmm12, 0x69
	vinserti64x2 zmm15, zmm8, xmm8, 0xd6
	vinserti64x2 ymm23, ymm25, xmm16, 0xd4
	vinserti64x2 zmm21, zmm19, xmm20, 0x93
	vinserti64x2 ymm3{k2}, ymm5, oword [0xa99], 0x2f
	vinserti64x2 zmm6{k2}, zmm4, xmm3, 0x3
	vinserti64x2 ymm5{k4}{z}, ymm3, xmm1, 0x31
	vinserti64x2 zmm1{k7}{z}, zmm1, xmm5, 0x60
	vinserti64x2 ymm7, ymm7, oword [eax+1], 0x5d
	vinserti64x2 ymm0, ymm5, oword [eax+64], 0x4e
	vinserti64x2 zmm2, zmm7, oword [eax+1], 0x33
	vinserti64x2 zmm0, zmm3, oword [eax+64], 0xaf
	vinserti64x2 ymm7, ymm7, [0x401], 0x77
	vinserti64x2 zmm4, zmm7, [0xf64], 0x2b
