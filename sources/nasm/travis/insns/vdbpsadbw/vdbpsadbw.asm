default rel
	vdbpsadbw xmm1, xmm3, xmm6, 0xe8
	vdbpsadbw xmm1, xmm6, 0xe8
	vdbpsadbw xmm3, xmm7, xmm4, 0x50
	vdbpsadbw ymm6, ymm6, yword [0x1d4], 0x19
	vdbpsadbw ymm6, yword [0x1d4], 0x19
	vdbpsadbw ymm6, ymm7, ymm3, 0xfc
	vdbpsadbw zmm2, zmm7, zmm0, 0xb0
	vdbpsadbw zmm2, zmm0, 0xb0
	vdbpsadbw zmm6, zmm1, zmm0, 0xa0
	vdbpsadbw xmm14, xmm8, xmm11, 0x9a
	vdbpsadbw ymm8, ymm14, ymm15, 0xf0
	vdbpsadbw zmm11, zmm10, zmm15, 0x95
	vdbpsadbw xmm27, xmm31, xmm19, 0x2e
	vdbpsadbw ymm20, ymm16, ymm28, 0x99
	vdbpsadbw zmm24, zmm28, zmm18, 0xb2
	vdbpsadbw xmm0{k3}, xmm2, xmm1, 0xdd
	vdbpsadbw ymm6{k3}, ymm7, ymm7, 0xd6
	vdbpsadbw zmm3{k6}, zmm6, zword [0x3ee], 0x7d
	vdbpsadbw xmm1{k5}{z}, xmm1, xmm7, 0x5a
	vdbpsadbw ymm1{k1}{z}, ymm4, ymm3, 0x19
	vdbpsadbw zmm4{k5}{z}, zmm0, zword [0xfcb], 0xf6
	vdbpsadbw xmm3, xmm7, oword [eax+1], 0xee
	vdbpsadbw xmm6, xmm6, oword [eax+64], 0x6f
	vdbpsadbw ymm7, ymm6, yword [eax+1], 0x65
	vdbpsadbw ymm2, ymm6, yword [eax+64], 0xad
	vdbpsadbw zmm0, zmm1, zword [eax+1], 0x3b
	vdbpsadbw zmm1, zmm2, zword [eax+64], 0x1e
	vdbpsadbw xmm7, xmm0, [0xff0], 0x15
	vdbpsadbw ymm3, ymm5, [0xc82], 0x75
	vdbpsadbw zmm4, zmm0, [0x16b], 0xf5
