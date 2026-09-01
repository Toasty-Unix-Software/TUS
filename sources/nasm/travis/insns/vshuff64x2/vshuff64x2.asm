default rel
	vshuff64x2 ymm1, ymm7, yword [0x2f7], 0x98
	vshuff64x2 ymm1, yword [0x2f7], 0x98
	vshuff64x2 ymm7, ymm1, ymm6, 0x29
	vshuff64x2 zmm5, zmm6, zmm1, 0x3c
	vshuff64x2 zmm5, zmm1, 0x3c
	vshuff64x2 zmm1, zmm6, zmm6, 0xd4
	vshuff64x2 ymm15, ymm11, ymm14, 0xfd
	vshuff64x2 zmm15, zmm14, zmm9, 0x57
	vshuff64x2 ymm20, ymm24, ymm17, 0x15
	vshuff64x2 zmm21, zmm19, zmm18, 0xf2
	vshuff64x2 ymm4{k4}, ymm6, ymm0, 0xab
	vshuff64x2 zmm7{k3}, zmm5, zword [0x26d], 0x3a
	vshuff64x2 ymm7{k3}{z}, ymm1, ymm5, 0x7a
	vshuff64x2 zmm4{k2}{z}, zmm5, zmm5, 0xc1
	vshuff64x2 ymm2, ymm7, yword [eax+1], 0x90
	vshuff64x2 ymm5, ymm3, yword [eax+64], 0x19
	vshuff64x2 zmm6, zmm0, zword [eax+1], 0xc1
	vshuff64x2 zmm0, zmm7, zword [eax+64], 0xcc
	vshuff64x2 ymm2, ymm2, [0x588], 0xbd
	vshuff64x2 zmm3, zmm5, [0x728], 0x93
