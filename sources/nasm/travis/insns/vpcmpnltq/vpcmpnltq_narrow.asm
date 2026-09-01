	vpcmpnltq k4, xmm6, xmm2
	vpcmpnltq k2, xmm0, xmm6
	vpcmpnltq k7, ymm5, ymm0
	vpcmpnltq k5, ymm6, ymm4
	vpcmpnltq k4, zmm5, zword [0xc12]
	vpcmpnltq k1, zmm1, zmm7

%ifdef ERROR
	vpcmpnltq k7, xmm8, xmm9
	vpcmpnltq k5, ymm13, ymm9
	vpcmpnltq k6, zmm11, zmm11
	vpcmpnltq k6, xmm21, xmm30
	vpcmpnltq k7, ymm25, ymm17
	vpcmpnltq k3, zmm23, zmm25
%endif
