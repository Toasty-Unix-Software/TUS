default rel
	valignq xmm5, xmm4, xmm4, 0xd7
	valignq xmm5, xmm4, 0xd7
	valignq xmm3, xmm2, xmm3, 0x9c
	valignq ymm7, ymm1, ymm6, 0xa
	valignq ymm7, ymm6, 0xa
	valignq ymm6, ymm6, yword [0x82d], 0x8e
	valignq zmm1, zmm5, zword [0x194], 0x2e
	valignq zmm1, zword [0x194], 0x2e
	valignq zmm4, zmm1, zmm5, 0x78
	valignq xmm9, xmm14, xmm9, 0xaa
	valignq ymm14, ymm13, ymm13, 0x57
	valignq zmm15, zmm14, zmm12, 0x2e
	valignq xmm19, xmm21, xmm19, 0xa4
	valignq ymm24, ymm19, ymm29, 0x69
	valignq zmm19, zmm16, zmm16, 0xd8
	valignq xmm7{k2}, xmm2, xmm2, 0x89
	valignq ymm2{k1}, ymm7, yword [0x187], 0x7a
	valignq zmm7{k3}, zmm1, zmm5, 0x78
	valignq xmm6{k3}{z}, xmm3, oword [0xf0e], 0x84
	valignq ymm0{k7}{z}, ymm0, yword [0xa11], 0x95
	valignq zmm2{k7}{z}, zmm4, zmm7, 0x59
	valignq xmm5, xmm1, oword [eax+1], 0x82
	valignq xmm2, xmm6, oword [eax+64], 0x62
	valignq ymm4, ymm2, yword [eax+1], 0xee
	valignq ymm2, ymm2, yword [eax+64], 0x6f
	valignq zmm1, zmm2, zword [eax+1], 0x11
	valignq zmm6, zmm6, zword [eax+64], 0x23
	valignq xmm1, xmm3, [0x957], 0x4d
	valignq ymm4, ymm1, [0x9dc], 0xf
	valignq zmm1, zmm2, [0x316], 0xfc
