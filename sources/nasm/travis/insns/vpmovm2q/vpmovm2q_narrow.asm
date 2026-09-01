	vpmovm2q xmm2, k7
	vpmovm2q xmm6, k5
	vpmovm2q ymm0, k6
	vpmovm2q ymm1, k4
	vpmovm2q zmm4, k7
	vpmovm2q zmm5, k6

%ifdef ERROR
	vpmovm2q xmm13, k2
	vpmovm2q ymm12, k7
	vpmovm2q zmm14, k2
	vpmovm2q xmm23, k4
	vpmovm2q ymm27, k7
	vpmovm2q zmm21, k6
%endif
