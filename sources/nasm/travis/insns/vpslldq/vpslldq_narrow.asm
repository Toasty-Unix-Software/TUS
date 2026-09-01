	vpslldq xmm1, xmm3, 0xb8
	vpslldq xmm1, 0xb8
	vpslldq xmm6, xmm5, 0x75
	vpslldq ymm4, ymm3, 0x52
	vpslldq ymm4, 0x52
	vpslldq ymm6, ymm3, 0xcf
	vpslldq xmm5, xmm2, 0xad
	vpslldq xmm5, 0xad
	vpslldq xmm7, xmm6, 0x34
	vpslldq ymm4, ymm4, 0x47
	vpslldq ymm4, 0x47
	vpslldq ymm1, ymm5, 0xc9

%ifdef ERROR
	vpslldq xmm13, xmm13, 0x7f
	vpslldq ymm11, ymm15, 0x4b
	vpslldq xmm10, xmm10, 0x97
	vpslldq ymm13, ymm8, 0x25
	vpslldq xmm18, xmm29, 0x62
	vpslldq ymm30, ymm18, 0xa5
	vpslldq xmm30, xmm16, 0x55
	vpslldq ymm16, ymm21, 0x8c
%endif
