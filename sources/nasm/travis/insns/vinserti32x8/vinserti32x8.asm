default rel
	vinserti32x8 zmm5, zmm3, ymm0, 0x17
	vinserti32x8 zmm5, ymm0, 0x17
	vinserti32x8 zmm2, zmm6, ymm5, 0x4c
	vinserti32x8 zmm15, zmm9, ymm8, 0x2d
	vinserti32x8 zmm16, zmm29, ymm23, 0xf1
	vinserti32x8 zmm7{k4}, zmm3, ymm5, 0xae
	vinserti32x8 zmm3{k7}{z}, zmm1, yword [0xcbc], 0x4c
	vinserti32x8 zmm2, zmm4, yword [eax+1], 0x3b
	vinserti32x8 zmm2, zmm6, yword [eax+64], 0xd5
	vinserti32x8 zmm3, zmm6, [0x421], 0x25
