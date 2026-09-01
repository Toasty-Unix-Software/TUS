	vpgatherqq xmm6, [xmm6*1], xmm0
	vpgatherqq xmm4, [xmm6*1], xmm6
	vpgatherqq ymm5, [ymm7*1], ymm3
	vpgatherqq ymm0, [ymm4*1], ymm1
	vpgatherqq xmm3, [xmm0*1]
	vpgatherqq xmm2, [xmm5*1]
	vpgatherqq ymm4, [ymm6*1]
	vpgatherqq ymm3, [ymm6*1]

%ifdef ERROR
	vpgatherqq xmm8, [xmm15*1], xmm8
	vpgatherqq ymm14, [ymm11*1], ymm14
	vpgatherqq xmm15, [xmm10*1]
	vpgatherqq ymm13, [ymm14*1]
%endif
