default rel
	vreduceps xmm0, oword [0x938], 0x28
	vreduceps xmm7, xmm1, 0x4f
	vreduceps ymm5, ymm2, 0x87
	vreduceps ymm6, ymm5, 0xea
	vreduceps zmm1, zmm6, 0x4b
	vreduceps zmm0, zmm2, 0x5f
	vreduceps xmm9, xmm10, 0x6d
	vreduceps ymm9, ymm14, 0xf9
	vreduceps zmm15, zmm15, 0xab
	vreduceps xmm25, xmm17, 0xc6
	vreduceps ymm21, ymm23, 0xcd
	vreduceps zmm26, zmm28, 0xa7
	vreduceps xmm2{k1}, xmm0, 0xd8
	vreduceps ymm4{k5}, ymm7, 0x7a
	vreduceps zmm7{k4}, zword [0x912], 0xdf
	vreduceps xmm4{k5}{z}, xmm2, 0x93
	vreduceps ymm4{k7}{z}, ymm4, 0x24
	vreduceps zmm4{k6}{z}, zmm6, 0x6
	vreduceps xmm4, oword [eax+1], 0xdd
	vreduceps xmm6, oword [eax+64], 0x7c
	vreduceps ymm5, yword [eax+1], 0x98
	vreduceps ymm6, yword [eax+64], 0xad
	vreduceps zmm5, zword [eax+1], 0xe1
	vreduceps zmm5, zword [eax+64], 0xef
	vreduceps xmm0, [0x1dd], 0xdb
	vreduceps ymm3, [0x80b], 0x94
	vreduceps zmm1, [0x88c], 0xd4
