	vpcmpgeq k4, xmm1, xmm5
	vpcmpgeq k7, xmm4, oword [0x4b3]
	vpcmpgeq k5, ymm3, ymm7
	vpcmpgeq k6, ymm6, ymm7
	vpcmpgeq k3, zmm5, zword [0xffe]
	vpcmpgeq k7, zmm3, zmm7

%ifdef ERROR
	vpcmpgeq k1, xmm11, xmm13
	vpcmpgeq k7, ymm12, ymm8
	vpcmpgeq k4, zmm10, zmm9
	vpcmpgeq k5, xmm21, xmm16
	vpcmpgeq k1, ymm25, ymm19
	vpcmpgeq k4, zmm17, zmm16
%endif
