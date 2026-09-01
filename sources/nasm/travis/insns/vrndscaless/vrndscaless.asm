default rel
	vrndscaless xmm4, xmm4, xmm0, 0xec
	vrndscaless xmm4, xmm0, 0xec
	vrndscaless xmm0, xmm5, xmm6, 0xde
	vrndscaless xmm15, xmm8, xmm10, 0xa
	vrndscaless xmm31, xmm29, xmm26, 0x88
	vrndscaless xmm1{k6}, xmm7, xmm6, 0x27
	vrndscaless xmm0{k6}{z}, xmm5, xmm5, 0xb3
	vrndscaless xmm0, xmm7, dword [eax+1], 0xf4
	vrndscaless xmm1, xmm6, dword [eax+64], 0x7
	vrndscaless xmm7, xmm0, [0x985], 0xbd
