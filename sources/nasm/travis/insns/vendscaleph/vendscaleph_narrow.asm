	vendscaleph xmm4, xmm7, 0xe1
	vendscaleph xmm7, xmm7, 0x53
	vendscaleph ymm3, ymm5, 0x15
	vendscaleph ymm5, ymm2, 0x80
	vendscaleph zmm7, zmm4, 0xe
	vendscaleph zmm4, zmm5, 0x2c

%ifdef ERROR
	vendscaleph xmm10, xmm13, 0x69
	vendscaleph ymm9, ymm11, 0x61
	vendscaleph zmm8, zmm10, 0xca
	vendscaleph xmm30, xmm21, 0xd4
	vendscaleph ymm31, ymm30, 0x69
	vendscaleph zmm20, zmm22, 0x2
%endif
