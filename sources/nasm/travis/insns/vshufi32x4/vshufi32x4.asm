default rel
	vshufi32x4 ymm3, ymm1, yword [0x38c], 0xd7
	vshufi32x4 ymm3, yword [0x38c], 0xd7
	vshufi32x4 ymm4, ymm6, ymm2, 0x3a
	vshufi32x4 zmm2, zmm3, zword [0xadd], 0x30
	vshufi32x4 zmm2, zword [0xadd], 0x30
	vshufi32x4 zmm7, zmm2, zmm1, 0x43
	vshufi32x4 ymm14, ymm14, ymm8, 0x91
	vshufi32x4 zmm10, zmm13, zmm13, 0xd3
	vshufi32x4 ymm30, ymm17, ymm20, 0x69
	vshufi32x4 zmm17, zmm18, zmm30, 0xf3
	vshufi32x4 ymm5{k3}, ymm7, yword [0xacd], 0x2b
	vshufi32x4 zmm2{k2}, zmm0, zmm2, 0x24
	vshufi32x4 ymm7{k4}{z}, ymm2, ymm6, 0xa1
	vshufi32x4 zmm6{k5}{z}, zmm0, zmm7, 0xe0
	vshufi32x4 ymm0, ymm4, yword [eax+1], 0xd8
	vshufi32x4 ymm5, ymm7, yword [eax+64], 0xa2
	vshufi32x4 zmm4, zmm2, zword [eax+1], 0x95
	vshufi32x4 zmm1, zmm7, zword [eax+64], 0x7d
	vshufi32x4 ymm6, ymm5, [0xdd6], 0x4e
	vshufi32x4 zmm5, zmm2, [0xd6c], 0xef
