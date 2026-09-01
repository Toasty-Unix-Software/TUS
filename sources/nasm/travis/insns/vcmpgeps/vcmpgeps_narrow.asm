	vcmpgeps xmm3, xmm1, oword [0xf31]
	vcmpgeps xmm3, oword [0xf31]
	vcmpgeps xmm1, xmm5, oword [0x9ba]
	vcmpgeps ymm2, ymm3, ymm7
	vcmpgeps ymm2, ymm7
	vcmpgeps ymm1, ymm1, yword [0x14e]
	vcmpgeps k7, xmm3, xmm4
	vcmpgeps k4, xmm6, xmm1
	vcmpgeps k3, ymm0, ymm0
	vcmpgeps k1, ymm3, ymm6

%ifdef ERROR
	vcmpgeps xmm15, xmm15, xmm15
	vcmpgeps ymm13, ymm9, ymm15
	vcmpgeps k1, xmm8, xmm12
	vcmpgeps k5, ymm13, ymm14
%endif
