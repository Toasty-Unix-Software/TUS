	vpmovm2w xmm1, k6
	vpmovm2w xmm5, k1
	vpmovm2w ymm7, k5
	vpmovm2w ymm5, k1
	vpmovm2w zmm1, k3
	vpmovm2w zmm1, k4

%ifdef ERROR
	vpmovm2w xmm11, k4
	vpmovm2w ymm8, k3
	vpmovm2w zmm12, k5
	vpmovm2w xmm27, k4
	vpmovm2w ymm18, k4
	vpmovm2w zmm25, k1
%endif
