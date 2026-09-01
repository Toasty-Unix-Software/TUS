default rel
	vmovqqa ymm5, ymm3
	vmovqqa ymm1, yword [0x4b2]
	vmovqqa yword [0x135], ymm0
	vmovqqa ymm0, ymm7
	vmovqqa ymm15, ymm10
	vmovqqa ymm8, ymm11
	vmovqqa ymm7, yword [eax+1]
	vmovqqa ymm3, yword [eax+64]
	vmovqqa yword [eax+1], ymm1
	vmovqqa yword [eax+64], ymm3
	vmovqqa ymm2, [0x54c]
	vmovqqa [0xcbe], ymm6
