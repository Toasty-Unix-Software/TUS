	vpshldvd xmm2, xmm7, oword [0xf31]
	vpshldvd xmm2, oword [0xf31]
	vpshldvd xmm4, xmm4, oword [0x2e7]
	vpshldvd ymm7, ymm2, yword [0xe58]
	vpshldvd ymm7, yword [0xe58]
	vpshldvd ymm7, ymm0, yword [0xf81]
	vpshldvd zmm0, zmm2, zmm7
	vpshldvd zmm0, zmm7
	vpshldvd zmm6, zmm7, zword [0xd04]

%ifdef ERROR
	vpshldvd xmm15, xmm11, xmm15
	vpshldvd ymm15, ymm8, ymm13
	vpshldvd zmm9, zmm8, zmm8
	vpshldvd xmm26, xmm17, xmm26
	vpshldvd ymm18, ymm16, ymm26
	vpshldvd zmm22, zmm28, zmm20
%endif
