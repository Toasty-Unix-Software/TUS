	vpmovm2b xmm0, k2
	vpmovm2b xmm0, k7
	vpmovm2b ymm6, k7
	vpmovm2b ymm3, k7
	vpmovm2b zmm1, k3
	vpmovm2b zmm6, k1

%ifdef ERROR
	vpmovm2b xmm13, k6
	vpmovm2b ymm11, k7
	vpmovm2b zmm9, k1
	vpmovm2b xmm26, k6
	vpmovm2b ymm31, k1
	vpmovm2b zmm28, k2
%endif
