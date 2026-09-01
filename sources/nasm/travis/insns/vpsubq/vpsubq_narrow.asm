	vpsubq xmm5, xmm5, xmm0
	vpsubq xmm5, xmm0
	vpsubq xmm7, xmm3, oword [0x829]
	vpsubq ymm5, ymm7, ymm2
	vpsubq ymm5, ymm2
	vpsubq ymm7, ymm7, ymm3
	vpsubq xmm0, xmm1, oword [0xc55]
	vpsubq xmm0, oword [0xc55]
	vpsubq xmm2, xmm4, xmm6
	vpsubq ymm4, ymm4, ymm1
	vpsubq ymm4, ymm1
	vpsubq ymm2, ymm3, ymm1

%ifdef ERROR
	vpsubq xmm15, xmm8, xmm13
	vpsubq ymm14, ymm11, ymm11
	vpsubq xmm13, xmm10, xmm10
	vpsubq ymm15, ymm8, ymm14
	vpsubq xmm16, xmm28, xmm30
	vpsubq ymm25, ymm31, ymm29
	vpsubq xmm26, xmm22, xmm20
	vpsubq ymm28, ymm30, ymm23
%endif
