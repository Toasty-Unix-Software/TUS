	vpgatherqd xmm6, [xmm6*1], xmm6
	vpgatherqd xmm4, [xmm2*1], xmm0
	vpgatherqd xmm3, [ymm5*1], xmm3
	vpgatherqd xmm7, [ymm4*1], xmm7
	vpgatherqd xmm6, [xmm3*1]
	vpgatherqd xmm6, [xmm3*1]
	vpgatherqd xmm6, [ymm6*1]
	vpgatherqd xmm6, [ymm6*1]

%ifdef ERROR
	vpgatherqd xmm14, [xmm13*1], xmm14
	vpgatherqd xmm12, [ymm10*1], xmm14
	vpgatherqd xmm13, [xmm10*1]
	vpgatherqd xmm10, [ymm10*1]
%endif
