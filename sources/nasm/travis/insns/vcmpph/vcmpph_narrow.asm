	vcmpph k3, xmm1, xmm0, 0xc7
	vcmpph k3, xmm0, 0xc7
	vcmpph k1, xmm6, xmm0, 0x90
	vcmpph k1, ymm0, yword [0x6c9], 0x61
	vcmpph k1, yword [0x6c9], 0x61
	vcmpph k6, ymm4, ymm3, 0xe6
	vcmpph k4, zmm4, zmm3, 0xc
	vcmpph k4, zmm3, 0xc
	vcmpph k3, zmm4, zword [0x8a9], 0x3e

%ifdef ERROR
	vcmpph k4, xmm9, xmm10, 0x4b
	vcmpph k2, ymm15, ymm9, 0x1e
	vcmpph k6, zmm15, zmm10, 0xe9
	vcmpph k1, xmm28, xmm16, 0x1e
	vcmpph k3, ymm27, ymm31, 0x3e
	vcmpph k6, zmm25, zmm27, 0x62
%endif
