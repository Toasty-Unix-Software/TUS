	vpmadd52luq xmm4, xmm0, xmm2
	vpmadd52luq xmm2, xmm7, xmm1
	vpmadd52luq ymm7, ymm2, ymm5
	vpmadd52luq ymm4, ymm2, ymm6
	vpmadd52luq xmm0, xmm5, xmm4
	vpmadd52luq xmm3, xmm0, xmm7
	vpmadd52luq ymm5, ymm4, ymm0
	vpmadd52luq ymm4, ymm5, ymm1

%ifdef ERROR
	vpmadd52luq xmm8, xmm10, xmm12
	vpmadd52luq ymm14, ymm15, ymm15
	vpmadd52luq xmm11, xmm12, xmm10
	vpmadd52luq ymm9, ymm14, ymm12
	vpmadd52luq xmm16, xmm22, xmm30
	vpmadd52luq ymm21, ymm23, ymm20
	vpmadd52luq xmm30, xmm30, xmm29
	vpmadd52luq ymm24, ymm29, ymm22
%endif
