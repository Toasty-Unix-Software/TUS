	vrndscaless xmm4, xmm4, xmm0, 0xec
	vrndscaless xmm4, xmm0, 0xec
	vrndscaless xmm0, xmm5, xmm6, 0xde

%ifdef ERROR
	vrndscaless xmm15, xmm8, xmm10, 0xa
	vrndscaless xmm31, xmm29, xmm26, 0x88
%endif
