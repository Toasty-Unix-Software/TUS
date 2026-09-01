default rel
	vrangeps xmm6, xmm1, xmm2, 0x2
	vrangeps xmm6, xmm2, 0x2
	vrangeps xmm1, xmm7, oword [0xf6f], 0xa7
	vrangeps ymm1, ymm3, ymm4, 0xfd
	vrangeps ymm1, ymm4, 0xfd
	vrangeps ymm6, ymm0, ymm0, 0x8
	vrangeps zmm3, zmm5, zword [0xb22], 0x42
	vrangeps zmm3, zword [0xb22], 0x42
	vrangeps zmm3, zmm2, zword [0xe4d], 0xd9
	vrangeps xmm10, xmm11, xmm9, 0x49
	vrangeps ymm13, ymm14, ymm14, 0xa5
	vrangeps zmm11, zmm11, zmm15, 0x8d
	vrangeps xmm27, xmm29, xmm28, 0x23
	vrangeps ymm24, ymm17, ymm30, 0x9c
	vrangeps zmm30, zmm26, zmm17, 0xc4
	vrangeps xmm7{k1}, xmm1, oword [0xaff], 0x61
	vrangeps ymm6{k3}, ymm6, ymm0, 0x4
	vrangeps zmm6{k6}, zmm1, zmm7, 0x1d
	vrangeps xmm6{k1}{z}, xmm0, xmm6, 0xd1
	vrangeps ymm3{k7}{z}, ymm3, yword [0x7fa], 0x8b
	vrangeps zmm1{k5}{z}, zmm4, zword [0x9f7], 0x76
	vrangeps xmm0, xmm6, oword [eax+1], 0xc6
	vrangeps xmm3, xmm6, oword [eax+64], 0xc5
	vrangeps ymm7, ymm0, yword [eax+1], 0xc
	vrangeps ymm3, ymm7, yword [eax+64], 0x28
	vrangeps zmm2, zmm1, zword [eax+1], 0x54
	vrangeps zmm2, zmm1, zword [eax+64], 0xaa
	vrangeps xmm7, xmm3, [0x4b4], 0x82
	vrangeps ymm3, ymm0, [0xd0e], 0x7a
	vrangeps zmm5, zmm0, [0x67c], 0x28
