	vreduceps xmm0, oword [0x938], 0x28
	vreduceps xmm7, xmm1, 0x4f
	vreduceps ymm5, ymm2, 0x87
	vreduceps ymm6, ymm5, 0xea
	vreduceps zmm1, zmm6, 0x4b
	vreduceps zmm0, zmm2, 0x5f

%ifdef ERROR
	vreduceps xmm9, xmm10, 0x6d
	vreduceps ymm9, ymm14, 0xf9
	vreduceps zmm15, zmm15, 0xab
	vreduceps xmm25, xmm17, 0xc6
	vreduceps ymm21, ymm23, 0xcd
	vreduceps zmm26, zmm28, 0xa7
%endif
