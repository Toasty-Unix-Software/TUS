	vpmovm2d xmm6, k1
	vpmovm2d xmm4, k6
	vpmovm2d ymm3, k7
	vpmovm2d ymm7, k2
	vpmovm2d zmm4, k7
	vpmovm2d zmm6, k5

%ifdef ERROR
	vpmovm2d xmm8, k1
	vpmovm2d ymm15, k7
	vpmovm2d zmm10, k4
	vpmovm2d xmm19, k4
	vpmovm2d ymm29, k1
	vpmovm2d zmm31, k3
%endif
