default rel
	vcmpbf16 k4, xmm2, xmm4, 0x8b
	vcmpbf16 k1, xmm0, xmm4, 0xd4
	vcmpbf16 k5, ymm6, yword [0x76e], 0x48
	vcmpbf16 k3, ymm1, ymm6, 0x3f
	vcmpbf16 k5, zmm7, zword [0x352], 0x4
	vcmpbf16 k6, zmm1, zword [0x68c], 0x60
	vcmpbf16 k5, xmm15, xmm14, 0x24
	vcmpbf16 k4, ymm11, ymm10, 0x1e
	vcmpbf16 k5, zmm13, zmm13, 0x35
	vcmpbf16 k2, xmm17, xmm23, 0x6
	vcmpbf16 k3, ymm19, ymm31, 0x2b
	vcmpbf16 k3, zmm30, zmm19, 0x75
	vcmpbf16 k7{k1}, xmm3, xmm4, 0x3
	vcmpbf16 k1{k7}, ymm0, ymm2, 0xcf
	vcmpbf16 k5{k5}, zmm0, zmm1, 0x53
	vcmpbf16 k5, xmm2, oword [eax+1], 0x2b
	vcmpbf16 k6, xmm6, oword [eax+64], 0x7
	vcmpbf16 k1, ymm0, yword [eax+1], 0x8c
	vcmpbf16 k1, ymm5, yword [eax+64], 0xfe
	vcmpbf16 k1, zmm6, zword [eax+1], 0xe5
	vcmpbf16 k6, zmm1, zword [eax+64], 0x16
	vcmpbf16 k3, xmm2, [0x4bf], 0x2
	vcmpbf16 k7, ymm4, [0x25f], 0x10
	vcmpbf16 k4, zmm5, [0xcd7], 0x2e
