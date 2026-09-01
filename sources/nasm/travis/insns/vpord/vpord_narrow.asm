	vpord xmm2, xmm2, xmm0
	vpord xmm2, xmm0
	vpord xmm0, xmm3, xmm1
	vpord ymm6, ymm1, ymm6
	vpord ymm6, ymm6
	vpord ymm6, ymm2, ymm6
	vpord zmm5, zmm0, zmm4
	vpord zmm5, zmm4
	vpord zmm7, zmm1, zmm6

%ifdef ERROR
	vpord xmm8, xmm15, xmm11
	vpord ymm15, ymm11, ymm12
	vpord zmm15, zmm12, zmm10
	vpord xmm28, xmm26, xmm21
	vpord ymm26, ymm16, ymm25
	vpord zmm21, zmm17, zmm27
%endif
