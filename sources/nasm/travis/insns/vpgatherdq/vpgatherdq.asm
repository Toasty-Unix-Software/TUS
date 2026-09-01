default rel
	vpgatherdq xmm3, [xmm5*1], xmm7
	vpgatherdq xmm6, [xmm5*1], xmm4
	vpgatherdq ymm7, [xmm1*1], ymm2
	vpgatherdq ymm4, [xmm7*1], ymm4
	vpgatherdq xmm4, [xmm5*1]
	vpgatherdq xmm2, [xmm0*1]
	vpgatherdq ymm3, [xmm5*1]
	vpgatherdq ymm2, [xmm4*1]
	vpgatherdq xmm15, [xmm9*1], xmm11
	vpgatherdq ymm10, [xmm8*1], ymm14
	vpgatherdq xmm12, [xmm14*1]
	vpgatherdq ymm10, [xmm11*1]
	vpgatherdq xmm1{k1}, [xmm6*1]
	vpgatherdq ymm4{k1}, [xmm4*1]
	vpgatherdq zmm4{k5}, [ymm4*1]
