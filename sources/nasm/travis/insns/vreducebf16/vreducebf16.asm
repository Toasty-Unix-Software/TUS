default rel
	vreducebf16 xmm5, xmm6, 0x88
	vreducebf16 xmm1, xmm2, 0x1f
	vreducebf16 ymm1, ymm4, 0x2f
	vreducebf16 ymm7, ymm0, 0xde
	vreducebf16 zmm3, zmm2, 0x5a
	vreducebf16 zmm3, zword [0x9f7], 0xd2
	vreducebf16 xmm10, xmm14, 0x8c
	vreducebf16 ymm13, ymm8, 0x86
	vreducebf16 zmm11, zmm13, 0xfa
	vreducebf16 xmm16, xmm16, 0x7a
	vreducebf16 ymm17, ymm19, 0x2
	vreducebf16 zmm22, zmm16, 0x9e
	vreducebf16 xmm7{k5}, oword [0xbce], 0xd0
	vreducebf16 ymm1{k4}, ymm7, 0x8e
	vreducebf16 zmm3{k5}, zmm1, 0xc9
	vreducebf16 xmm7{k1}{z}, xmm1, 0xa1
	vreducebf16 ymm4{k1}{z}, ymm7, 0x53
	vreducebf16 zmm1{k4}{z}, zword [0x4cf], 0x41
	vreducebf16 xmm0, oword [eax+1], 0x9f
	vreducebf16 xmm7, oword [eax+64], 0x90
	vreducebf16 ymm0, yword [eax+1], 0x95
	vreducebf16 ymm5, yword [eax+64], 0xb5
	vreducebf16 zmm6, zword [eax+1], 0xb2
	vreducebf16 zmm3, zword [eax+64], 0x4c
	vreducebf16 xmm2, [0xcf0], 0xae
	vreducebf16 ymm2, [0x4cb], 0x50
	vreducebf16 zmm0, [0x3bc], 0x44
