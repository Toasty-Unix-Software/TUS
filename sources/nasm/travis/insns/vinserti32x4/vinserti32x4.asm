default rel
	vinserti32x4 ymm2, ymm0, xmm1, 0x4e
	vinserti32x4 ymm2, xmm1, 0x4e
	vinserti32x4 ymm6, ymm4, oword [0xfa7], 0x85
	vinserti32x4 zmm2, zmm3, xmm4, 0x4f
	vinserti32x4 zmm2, xmm4, 0x4f
	vinserti32x4 zmm0, zmm1, oword [0x72e], 0x29
	vinserti32x4 ymm12, ymm9, xmm13, 0x6a
	vinserti32x4 zmm9, zmm8, xmm12, 0x5e
	vinserti32x4 ymm25, ymm22, xmm31, 0x3e
	vinserti32x4 zmm21, zmm17, xmm24, 0xa1
	vinserti32x4 ymm5{k5}, ymm2, oword [0x21f], 0x36
	vinserti32x4 zmm5{k7}, zmm7, xmm3, 0x6
	vinserti32x4 ymm0{k1}{z}, ymm2, oword [0x85a], 0x3a
	vinserti32x4 zmm0{k4}{z}, zmm5, xmm6, 0xaf
	vinserti32x4 ymm4, ymm0, oword [eax+1], 0xa2
	vinserti32x4 ymm1, ymm1, oword [eax+64], 0xcd
	vinserti32x4 zmm0, zmm6, oword [eax+1], 0xad
	vinserti32x4 zmm6, zmm0, oword [eax+64], 0x67
	vinserti32x4 ymm0, ymm6, [0x309], 0x9c
	vinserti32x4 zmm3, zmm2, [0x59d], 0xdc
