	vpmadd52huq xmm2, xmm6, xmm4
	vpmadd52huq xmm4, xmm5, oword [0xa0b]
	vpmadd52huq ymm4, ymm7, yword [0xe8f]
	vpmadd52huq ymm7, ymm5, ymm0
	vpmadd52huq xmm6, xmm0, xmm4
	vpmadd52huq xmm2, xmm6, xmm4
	vpmadd52huq ymm1, ymm3, ymm7
	vpmadd52huq ymm0, ymm1, ymm7

%ifdef ERROR
	vpmadd52huq xmm9, xmm12, xmm10
	vpmadd52huq ymm14, ymm10, ymm10
	vpmadd52huq xmm12, xmm13, xmm10
	vpmadd52huq ymm12, ymm15, ymm8
	vpmadd52huq xmm23, xmm24, xmm24
	vpmadd52huq ymm20, ymm16, ymm17
	vpmadd52huq xmm22, xmm27, xmm31
	vpmadd52huq ymm28, ymm26, ymm25
%endif
