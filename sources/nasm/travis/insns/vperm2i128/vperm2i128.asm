default rel
	vperm2i128 ymm4, ymm0, ymm6, 0x7e
	vperm2i128 ymm4, ymm6, 0x7e
	vperm2i128 ymm7, ymm6, ymm0, 0x62
	vperm2i128 ymm11, ymm13, ymm11, 0x6e
	vperm2i128 ymm0, ymm1, yword [eax+1], 0xe9
	vperm2i128 ymm4, ymm4, yword [eax+64], 0x23
	vperm2i128 ymm5, ymm2, [0xfc4], 0x85
