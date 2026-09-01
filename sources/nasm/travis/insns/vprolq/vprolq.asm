default rel
	vprolq xmm0, xmm6, 0x1e
	vprolq xmm0, 0x1e
	vprolq xmm6, oword [0x377], 0x7a
	vprolq ymm0, ymm5, 0x7c
	vprolq ymm0, 0x7c
	vprolq ymm0, ymm4, 0xa
	vprolq zmm3, zmm5, 0x8c
	vprolq zmm3, 0x8c
	vprolq zmm3, zword [0xadf], 0xbc
	vprolq xmm10, xmm9, 0x5a
	vprolq ymm11, ymm10, 0x3a
	vprolq zmm15, zmm11, 0x4e
	vprolq xmm24, xmm22, 0xf1
	vprolq ymm20, ymm28, 0xf
	vprolq zmm23, zmm28, 0x51
	vprolq xmm7{k2}, xmm3, 0x62
	vprolq ymm1{k1}, ymm4, 0x9f
	vprolq zmm5{k4}, zmm4, 0x1
	vprolq xmm7{k1}{z}, xmm6, 0x68
	vprolq ymm1{k6}{z}, ymm6, 0xff
	vprolq zmm1{k3}{z}, zword [0xf10], 0x95
	vprolq xmm4, oword [eax+1], 0xbe
	vprolq xmm6, oword [eax+64], 0x92
	vprolq ymm1, yword [eax+1], 0x46
	vprolq ymm6, yword [eax+64], 0xe2
	vprolq zmm1, zword [eax+1], 0x13
	vprolq zmm2, zword [eax+64], 0x74
	vprolq xmm0, [0x39c], 0x47
	vprolq ymm1, [0x532], 0xe6
	vprolq zmm1, [0xa11], 0x29
