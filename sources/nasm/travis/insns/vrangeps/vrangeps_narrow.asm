	vrangeps xmm6, xmm1, xmm2, 0x2
	vrangeps xmm6, xmm2, 0x2
	vrangeps xmm1, xmm7, oword [0xf6f], 0xa7
	vrangeps ymm1, ymm3, ymm4, 0xfd
	vrangeps ymm1, ymm4, 0xfd
	vrangeps ymm6, ymm0, ymm0, 0x8
	vrangeps zmm3, zmm5, zword [0xb22], 0x42
	vrangeps zmm3, zword [0xb22], 0x42
	vrangeps zmm3, zmm2, zword [0xe4d], 0xd9

%ifdef ERROR
	vrangeps xmm10, xmm11, xmm9, 0x49
	vrangeps ymm13, ymm14, ymm14, 0xa5
	vrangeps zmm11, zmm11, zmm15, 0x8d
	vrangeps xmm27, xmm29, xmm28, 0x23
	vrangeps ymm24, ymm17, ymm30, 0x9c
	vrangeps zmm30, zmm26, zmm17, 0xc4
%endif
