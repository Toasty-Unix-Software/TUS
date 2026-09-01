default rel
	vshufi64x2 ymm0, ymm5, yword [0x510], 0x14
	vshufi64x2 ymm0, yword [0x510], 0x14
	vshufi64x2 ymm5, ymm1, yword [0x201], 0xfe
	vshufi64x2 zmm7, zmm4, zmm7, 0xf4
	vshufi64x2 zmm7, zmm7, 0xf4
	vshufi64x2 zmm5, zmm6, zmm3, 0xf0
	vshufi64x2 ymm11, ymm9, ymm13, 0x1e
	vshufi64x2 zmm10, zmm14, zmm14, 0x3c
	vshufi64x2 ymm30, ymm18, ymm17, 0x5b
	vshufi64x2 zmm26, zmm18, zmm22, 0xd0
	vshufi64x2 ymm5{k7}, ymm0, ymm3, 0x5b
	vshufi64x2 zmm2{k2}, zmm5, zmm6, 0x82
	vshufi64x2 ymm1{k4}{z}, ymm0, ymm2, 0xb5
	vshufi64x2 zmm5{k6}{z}, zmm7, zmm0, 0x55
	vshufi64x2 ymm6, ymm2, yword [eax+1], 0xb8
	vshufi64x2 ymm1, ymm5, yword [eax+64], 0x59
	vshufi64x2 zmm6, zmm1, zword [eax+1], 0xc3
	vshufi64x2 zmm2, zmm3, zword [eax+64], 0x7b
	vshufi64x2 ymm2, ymm4, [0xe3c], 0x7b
	vshufi64x2 zmm5, zmm1, [0x418], 0x7
