	vcmpbf16 k4, xmm2, xmm4, 0x8b
	vcmpbf16 k1, xmm0, xmm4, 0xd4
	vcmpbf16 k5, ymm6, yword [0x76e], 0x48
	vcmpbf16 k3, ymm1, ymm6, 0x3f
	vcmpbf16 k5, zmm7, zword [0x352], 0x4
	vcmpbf16 k6, zmm1, zword [0x68c], 0x60

%ifdef ERROR
	vcmpbf16 k5, xmm15, xmm14, 0x24
	vcmpbf16 k4, ymm11, ymm10, 0x1e
	vcmpbf16 k5, zmm13, zmm13, 0x35
	vcmpbf16 k2, xmm17, xmm23, 0x6
	vcmpbf16 k3, ymm19, ymm31, 0x2b
	vcmpbf16 k3, zmm30, zmm19, 0x75
%endif
