	vpcmpltq k2, xmm1, xmm4
	vpcmpltq k2, xmm3, xmm3
	vpcmpltq k1, ymm3, ymm1
	vpcmpltq k6, ymm2, ymm3
	vpcmpltq k2, zmm2, zmm0
	vpcmpltq k6, zmm0, zword [0x98e]

%ifdef ERROR
	vpcmpltq k3, xmm15, xmm14
	vpcmpltq k7, ymm14, ymm14
	vpcmpltq k5, zmm13, zmm9
	vpcmpltq k5, xmm20, xmm25
	vpcmpltq k1, ymm24, ymm30
	vpcmpltq k7, zmm28, zmm28
%endif
