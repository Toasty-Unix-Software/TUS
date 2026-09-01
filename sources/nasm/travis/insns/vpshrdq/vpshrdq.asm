default rel
	vpshrdq xmm4, xmm0, xmm4, 0xf6
	vpshrdq xmm4, xmm4, 0xf6
	vpshrdq xmm3, xmm4, xmm7, 0x6c
	vpshrdq ymm0, ymm1, ymm2, 0x50
	vpshrdq ymm0, ymm2, 0x50
	vpshrdq ymm7, ymm5, ymm7, 0x20
	vpshrdq zmm3, zmm2, zword [0x93e], 0x39
	vpshrdq zmm3, zword [0x93e], 0x39
	vpshrdq zmm3, zmm0, zmm5, 0x61
	vpshrdq xmm15, xmm11, xmm8, 0x44
	vpshrdq ymm12, ymm9, ymm8, 0xd4
	vpshrdq zmm12, zmm8, zmm15, 0x6d
	vpshrdq xmm29, xmm23, xmm23, 0xcd
	vpshrdq ymm20, ymm27, ymm23, 0xf4
	vpshrdq zmm17, zmm24, zmm18, 0xc6
	vpshrdq xmm7{k7}, xmm1, xmm7, 0xf
	vpshrdq ymm1{k2}, ymm5, ymm4, 0xc4
	vpshrdq zmm7{k6}, zmm4, zmm4, 0xb9
	vpshrdq xmm7{k1}{z}, xmm3, xmm3, 0x6a
	vpshrdq ymm4{k5}{z}, ymm6, ymm3, 0x14
	vpshrdq zmm2{k1}{z}, zmm1, zmm5, 0x54
	vpshrdq xmm6, xmm7, oword [eax+1], 0x89
	vpshrdq xmm5, xmm1, oword [eax+64], 0xf0
	vpshrdq ymm1, ymm3, yword [eax+1], 0x4d
	vpshrdq ymm4, ymm3, yword [eax+64], 0xf4
	vpshrdq zmm6, zmm4, zword [eax+1], 0x6f
	vpshrdq zmm6, zmm5, zword [eax+64], 0xd2
	vpshrdq xmm2, xmm2, [0x87e], 0x1d
	vpshrdq ymm5, ymm2, [0x79c], 0xc6
	vpshrdq zmm2, zmm1, [0x4f0], 0x7d
