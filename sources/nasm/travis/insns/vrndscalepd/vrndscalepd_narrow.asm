	vrndscalepd xmm4, oword [0x1b1], 0x38
	vrndscalepd xmm2, xmm0, 0xd8
	vrndscalepd ymm3, ymm5, 0x34
	vrndscalepd ymm6, yword [0x791], 0xd9
	vrndscalepd zmm1, zmm5, 0x29
	vrndscalepd zmm2, zmm7, 0x74

%ifdef ERROR
	vrndscalepd xmm10, xmm14, 0xd1
	vrndscalepd ymm15, ymm10, 0x60
	vrndscalepd zmm15, zmm11, 0x96
	vrndscalepd xmm22, xmm18, 0x7c
	vrndscalepd ymm16, ymm26, 0x35
	vrndscalepd zmm26, zmm29, 0xdf
%endif
