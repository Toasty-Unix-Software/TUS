	vpshldvq xmm3, xmm6, xmm7
	vpshldvq xmm3, xmm7
	vpshldvq xmm7, xmm1, oword [0xbe7]
	vpshldvq ymm0, ymm6, yword [0xb27]
	vpshldvq ymm0, yword [0xb27]
	vpshldvq ymm4, ymm4, ymm6
	vpshldvq zmm0, zmm2, zmm6
	vpshldvq zmm0, zmm6
	vpshldvq zmm1, zmm3, zmm3

%ifdef ERROR
	vpshldvq xmm11, xmm12, xmm10
	vpshldvq ymm9, ymm12, ymm15
	vpshldvq zmm12, zmm13, zmm13
	vpshldvq xmm28, xmm30, xmm26
	vpshldvq ymm19, ymm16, ymm22
	vpshldvq zmm18, zmm29, zmm27
%endif
