default rel
	vfpclassps k1, xmm1, 0x39
	vfpclassps k2, oword [0xff3], 0xc3
	vfpclassps k5, yword [0x2d4], 0x8f
	vfpclassps k6, ymm2, 0x2c
	vfpclassps k2, zword [0xda9], 0xcb
	vfpclassps k5, zmm2, 0x61
	vfpclassps k5, xmm15, 0x15
	vfpclassps k6, ymm12, 0xe7
	vfpclassps k3, zmm15, 0x9
	vfpclassps k1, xmm16, 0xb3
	vfpclassps k6, ymm27, 0x57
	vfpclassps k3, zmm29, 0xda
	vfpclassps k4{k4}, oword [0x2a7], 0xaa
	vfpclassps k3{k5}, ymm2, 0xa7
	vfpclassps k4{k5}, zmm2, 0x76
	vfpclassps k2, oword [eax+1], 0xef
	vfpclassps k7, oword [eax+64], 0xe0
	vfpclassps k4, yword [eax+1], 0xa8
	vfpclassps k2, yword [eax+64], 0xbc
	vfpclassps k5, zword [eax+1], 0x20
	vfpclassps k7, zword [eax+64], 0x8
	vfpclassps k5, [0xe07], 0xf4
	vfpclassps k3, [0xc4e], 0x9
	vfpclassps k6, [0xcc7], 0x99
