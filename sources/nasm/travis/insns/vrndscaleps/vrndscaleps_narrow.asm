	vrndscaleps xmm7, xmm5, 0x34
	vrndscaleps xmm5, xmm6, 0xbd
	vrndscaleps ymm6, ymm2, 0xc3
	vrndscaleps ymm3, yword [0xe5e], 0x3a
	vrndscaleps zmm0, zword [0xe5c], 0x87
	vrndscaleps zmm2, zmm0, 0x5

%ifdef ERROR
	vrndscaleps xmm11, xmm9, 0xfb
	vrndscaleps ymm15, ymm9, 0x8c
	vrndscaleps zmm15, zmm10, 0x56
	vrndscaleps xmm24, xmm17, 0xea
	vrndscaleps ymm17, ymm19, 0xf1
	vrndscaleps zmm16, zmm31, 0xe2
%endif
