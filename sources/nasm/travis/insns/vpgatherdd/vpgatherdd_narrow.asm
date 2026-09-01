	vpgatherdd xmm2, [xmm5*1], xmm6
	vpgatherdd xmm0, [xmm1*1], xmm0
	vpgatherdd ymm3, [ymm4*1], ymm4
	vpgatherdd ymm6, [ymm0*1], ymm2
	vpgatherdd xmm4, [xmm5*1]
	vpgatherdd xmm1, [xmm5*1]
	vpgatherdd ymm3, [ymm2*1]
	vpgatherdd ymm5, [ymm3*1]

%ifdef ERROR
	vpgatherdd xmm9, [xmm14*1], xmm11
	vpgatherdd ymm11, [ymm11*1], ymm14
	vpgatherdd xmm8, [xmm15*1]
	vpgatherdd ymm11, [ymm8*1]
%endif
