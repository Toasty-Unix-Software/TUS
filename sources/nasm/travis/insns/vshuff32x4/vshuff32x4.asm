default rel
	vshuff32x4 ymm3, ymm6, ymm0, 0xa9
	vshuff32x4 ymm3, ymm0, 0xa9
	vshuff32x4 ymm5, ymm4, ymm1, 0xda
	vshuff32x4 zmm1, zmm7, zmm6, 0xec
	vshuff32x4 zmm1, zmm6, 0xec
	vshuff32x4 zmm4, zmm1, zmm0, 0xa9
	vshuff32x4 ymm9, ymm9, ymm10, 0xe9
	vshuff32x4 zmm8, zmm9, zmm9, 0x5d
	vshuff32x4 ymm20, ymm27, ymm18, 0x79
	vshuff32x4 zmm28, zmm20, zmm17, 0x95
	vshuff32x4 ymm2{k5}, ymm3, ymm5, 0x1f
	vshuff32x4 zmm2{k6}, zmm7, zmm5, 0xac
	vshuff32x4 ymm7{k4}{z}, ymm5, ymm1, 0x2b
	vshuff32x4 zmm5{k4}{z}, zmm5, zmm2, 0xf5
	vshuff32x4 ymm1, ymm7, yword [eax+1], 0x8b
	vshuff32x4 ymm4, ymm6, yword [eax+64], 0xe
	vshuff32x4 zmm1, zmm2, zword [eax+1], 0x6
	vshuff32x4 zmm6, zmm5, zword [eax+64], 0xf7
	vshuff32x4 ymm3, ymm4, [0x6b1], 0xa3
	vshuff32x4 zmm4, zmm1, [0x2b7], 0x4
