default rel
	vbmacxor16x16x16 ymm6, ymm3, yword [0xeb6]
	vbmacxor16x16x16 ymm4, ymm1, yword [0xba6]
	vbmacxor16x16x16 zmm2, zmm1, zword [0xe77]
	vbmacxor16x16x16 zmm7, zmm1, zmm6
	vbmacxor16x16x16 ymm13, ymm10, ymm10
	vbmacxor16x16x16 zmm9, zmm10, zmm10
	vbmacxor16x16x16 ymm24, ymm17, ymm20
	vbmacxor16x16x16 zmm29, zmm26, zmm21
	vbmacxor16x16x16 ymm7, ymm1, yword [eax+1]
	vbmacxor16x16x16 ymm0, ymm5, yword [eax+64]
	vbmacxor16x16x16 zmm7, zmm2, zword [eax+1]
	vbmacxor16x16x16 zmm6, zmm1, zword [eax+64]
	vbmacxor16x16x16 ymm6, ymm0, [0x506]
	vbmacxor16x16x16 zmm3, zmm0, [0x89e]
