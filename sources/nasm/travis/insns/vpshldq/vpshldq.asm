default rel
	vpshldq xmm0, xmm0, xmm2, 0xc9
	vpshldq xmm0, xmm2, 0xc9
	vpshldq xmm7, xmm6, oword [0xd1c], 0x52
	vpshldq ymm7, ymm5, yword [0x8e5], 0xaa
	vpshldq ymm7, yword [0x8e5], 0xaa
	vpshldq ymm0, ymm7, yword [0xf22], 0x83
	vpshldq zmm4, zmm3, zword [0x786], 0x4a
	vpshldq zmm4, zword [0x786], 0x4a
	vpshldq zmm4, zmm1, zmm5, 0xc9
	vpshldq xmm9, xmm12, xmm13, 0x22
	vpshldq ymm10, ymm10, ymm9, 0x1d
	vpshldq zmm8, zmm14, zmm10, 0x2d
	vpshldq xmm16, xmm31, xmm20, 0xa7
	vpshldq ymm20, ymm31, ymm24, 0xe
	vpshldq zmm24, zmm20, zmm20, 0xa
	vpshldq xmm0{k2}, xmm1, xmm6, 0x61
	vpshldq ymm0{k5}, ymm5, ymm3, 0xc7
	vpshldq zmm1{k3}, zmm0, zword [0xbcb], 0x18
	vpshldq xmm7{k1}{z}, xmm6, xmm5, 0x20
	vpshldq ymm4{k5}{z}, ymm2, ymm6, 0xf8
	vpshldq zmm5{k7}{z}, zmm2, zword [0x328], 0x89
	vpshldq xmm3, xmm4, oword [eax+1], 0x87
	vpshldq xmm2, xmm4, oword [eax+64], 0x73
	vpshldq ymm2, ymm7, yword [eax+1], 0xca
	vpshldq ymm1, ymm0, yword [eax+64], 0xfd
	vpshldq zmm1, zmm4, zword [eax+1], 0x54
	vpshldq zmm5, zmm4, zword [eax+64], 0xab
	vpshldq xmm1, xmm0, [0x93d], 0x72
	vpshldq ymm3, ymm6, [0xb32], 0x8b
	vpshldq zmm4, zmm5, [0x9e0], 0x4d
