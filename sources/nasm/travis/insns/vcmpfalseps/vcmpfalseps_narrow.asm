	vcmpfalseps xmm4, xmm4, xmm2
	vcmpfalseps xmm4, xmm2
	vcmpfalseps xmm0, xmm2, oword [0x959]
	vcmpfalseps ymm3, ymm5, yword [0x1b8]
	vcmpfalseps ymm3, yword [0x1b8]
	vcmpfalseps ymm1, ymm2, ymm3
	vcmpfalseps k3, xmm1, xmm1
	vcmpfalseps k7, xmm0, xmm2
	vcmpfalseps k7, ymm3, ymm6
	vcmpfalseps k3, ymm5, ymm7

%ifdef ERROR
	vcmpfalseps xmm10, xmm12, xmm11
	vcmpfalseps ymm11, ymm11, ymm10
	vcmpfalseps k3, xmm12, xmm10
	vcmpfalseps k7, ymm9, ymm8
%endif
