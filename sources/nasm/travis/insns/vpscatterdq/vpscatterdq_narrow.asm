	vpscatterdq [xmm4*1], xmm0
	vpscatterdq [xmm5*1], xmm4
	vpscatterdq [xmm0*1], ymm3
	vpscatterdq [xmm3*1], ymm3
	vpscatterdq [ymm3*1], zmm1
	vpscatterdq [ymm6*1], zmm5

%ifdef ERROR
	vpscatterdq [xmm11*1], xmm9
	vpscatterdq [xmm10*1], ymm8
	vpscatterdq [ymm10*1], zmm14
	vpscatterdq [xmm17*1], xmm24
	vpscatterdq [xmm24*1], ymm26
	vpscatterdq [ymm29*1], zmm24
%endif
