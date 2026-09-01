default rel
	vinsertf128 ymm0, ymm4, oword [0x3b2], 0x4e
	vinsertf128 ymm0, oword [0x3b2], 0x4e
	vinsertf128 ymm4, ymm7, oword [0x9ef], 0x30
	vinsertf128 ymm9, ymm11, xmm8, 0x18
	vinsertf128 ymm7, ymm7, oword [eax+1], 0xc2
	vinsertf128 ymm2, ymm6, oword [eax+64], 0x36
	vinsertf128 ymm7, ymm0, [0x83b], 0x9c
