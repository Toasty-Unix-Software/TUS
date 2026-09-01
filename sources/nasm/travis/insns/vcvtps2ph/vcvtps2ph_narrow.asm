	vcvtps2ph xmm4, ymm5, 0xe0
	vcvtps2ph xmm6, ymm7, 0xe8
	vcvtps2ph xmm5, xmm6, 0x22
	vcvtps2ph xmm3, xmm7, 0x2c
	vcvtps2ph xmm1, xmm7, 0x5d
	vcvtps2ph xmm4, xmm5, 0xab
	vcvtps2ph xmm0, ymm4, 0xb1
	vcvtps2ph xmm3, ymm0, 0xf0

%ifdef ERROR
	vcvtps2ph xmm11, ymm9, 0x5e
	vcvtps2ph xmm14, xmm11, 0x68
	vcvtps2ph xmm12, xmm14, 0x75
	vcvtps2ph xmm14, ymm15, 0xa9
	vcvtps2ph xmm18, ymm26, 0x43
	vcvtps2ph xmm25, xmm27, 0xe5
	vcvtps2ph xmm31, xmm16, 0xc4
	vcvtps2ph xmm18, ymm16, 0x12
%endif
