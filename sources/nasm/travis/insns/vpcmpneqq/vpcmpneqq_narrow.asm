	vpcmpneqq k6, xmm5, xmm4
	vpcmpneqq k7, xmm3, xmm5
	vpcmpneqq k1, ymm5, ymm7
	vpcmpneqq k1, ymm6, yword [0x816]
	vpcmpneqq k3, zmm6, zmm3
	vpcmpneqq k2, zmm4, zmm1

%ifdef ERROR
	vpcmpneqq k3, xmm11, xmm13
	vpcmpneqq k5, ymm8, ymm12
	vpcmpneqq k7, zmm15, zmm10
	vpcmpneqq k5, xmm17, xmm24
	vpcmpneqq k4, ymm24, ymm27
	vpcmpneqq k5, zmm16, zmm24
%endif
