default rel
	vperm2f128 ymm1, ymm7, ymm4, 0x13
	vperm2f128 ymm1, ymm4, 0x13
	vperm2f128 ymm5, ymm7, ymm2, 0xee
	vperm2f128 ymm14, ymm12, ymm15, 0x3
	vperm2f128 ymm6, ymm7, yword [eax+1], 0xd2
	vperm2f128 ymm6, ymm7, yword [eax+64], 0xd8
	vperm2f128 ymm4, ymm2, [0xc3d], 0x82
