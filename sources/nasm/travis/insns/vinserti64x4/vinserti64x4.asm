default rel
	vinserti64x4 zmm7, zmm0, ymm1, 0xfa
	vinserti64x4 zmm7, ymm1, 0xfa
	vinserti64x4 zmm5, zmm5, ymm1, 0xba
	vinserti64x4 zmm15, zmm8, ymm8, 0x7b
	vinserti64x4 zmm30, zmm25, ymm17, 0xf6
	vinserti64x4 zmm6{k1}, zmm5, ymm4, 0x2d
	vinserti64x4 zmm1{k1}{z}, zmm1, ymm0, 0x35
	vinserti64x4 zmm7, zmm6, yword [eax+1], 0x3
	vinserti64x4 zmm5, zmm6, yword [eax+64], 0x51
	vinserti64x4 zmm5, zmm4, [0x85f], 0xbf
