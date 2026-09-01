default rel
	vcmpph k3, xmm1, xmm0, 0xc7
	vcmpph k3, xmm0, 0xc7
	vcmpph k1, xmm6, xmm0, 0x90
	vcmpph k1, ymm0, yword [0x6c9], 0x61
	vcmpph k1, yword [0x6c9], 0x61
	vcmpph k6, ymm4, ymm3, 0xe6
	vcmpph k4, zmm4, zmm3, 0xc
	vcmpph k4, zmm3, 0xc
	vcmpph k3, zmm4, zword [0x8a9], 0x3e
	vcmpph k4, xmm9, xmm10, 0x4b
	vcmpph k2, ymm15, ymm9, 0x1e
	vcmpph k6, zmm15, zmm10, 0xe9
	vcmpph k1, xmm28, xmm16, 0x1e
	vcmpph k3, ymm27, ymm31, 0x3e
	vcmpph k6, zmm25, zmm27, 0x62
	vcmpph k4{k2}, xmm5, oword [0x8a9], 0xaf
	vcmpph k7{k5}, ymm3, yword [0x9a3], 0xed
	vcmpph k7{k2}, zmm1, zmm4, 0xe2
	vcmpph k2, xmm7, oword [eax+1], 0xd5
	vcmpph k2, xmm5, oword [eax+64], 0x80
	vcmpph k3, ymm7, yword [eax+1], 0x49
	vcmpph k1, ymm2, yword [eax+64], 0x79
	vcmpph k5, zmm4, zword [eax+1], 0xb3
	vcmpph k7, zmm1, zword [eax+64], 0xab
	vcmpph k3, xmm5, [0x2f8], 0x4f
	vcmpph k7, ymm3, [0x490], 0xb6
	vcmpph k2, zmm1, [0x357], 0x3f
