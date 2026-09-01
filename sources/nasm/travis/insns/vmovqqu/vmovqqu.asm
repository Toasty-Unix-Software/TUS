default rel
	vmovqqu ymm7, ymm7
	vmovqqu ymm3, yword [0x3d6]
	vmovqqu yword [0xe07], ymm6
	vmovqqu yword [0x37d], ymm3
	vmovqqu ymm13, ymm13
	vmovqqu ymm13, ymm10
	vmovqqu ymm2, yword [eax+1]
	vmovqqu ymm1, yword [eax+64]
	vmovqqu yword [eax+1], ymm4
	vmovqqu yword [eax+64], ymm7
	vmovqqu ymm3, [0x48e]
	vmovqqu [0xb86], ymm4
