default rel
	vreducepd xmm3, xmm1, 0xc3
	vreducepd xmm7, xmm5, 0xd5
	vreducepd ymm7, ymm6, 0xdf
	vreducepd ymm0, yword [0xcd2], 0xbe
	vreducepd zmm7, zmm6, 0x90
	vreducepd zmm0, zmm0, 0x18
	vreducepd xmm12, xmm14, 0x61
	vreducepd ymm10, ymm11, 0x22
	vreducepd zmm12, zmm15, 0xb4
	vreducepd xmm30, xmm18, 0x66
	vreducepd ymm22, ymm26, 0xae
	vreducepd zmm28, zmm16, 0xcf
	vreducepd xmm6{k1}, xmm6, 0x6f
	vreducepd ymm3{k1}, ymm3, 0x6f
	vreducepd zmm3{k6}, zmm3, 0x50
	vreducepd xmm3{k5}{z}, xmm6, 0xbb
	vreducepd ymm6{k2}{z}, yword [0x42e], 0xcc
	vreducepd zmm7{k5}{z}, zmm1, 0x89
	vreducepd xmm0, oword [eax+1], 0x9c
	vreducepd xmm1, oword [eax+64], 0x5a
	vreducepd ymm2, yword [eax+1], 0x4f
	vreducepd ymm3, yword [eax+64], 0xcc
	vreducepd zmm0, zword [eax+1], 0x48
	vreducepd zmm2, zword [eax+64], 0xc5
	vreducepd xmm1, [0x150], 0xca
	vreducepd ymm0, [0x98d], 0xd0
	vreducepd zmm1, [0x562], 0x1d
