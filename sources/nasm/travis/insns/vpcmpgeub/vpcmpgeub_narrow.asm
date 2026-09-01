	vpcmpgeub k7, xmm7, xmm6
	vpcmpgeub k4, xmm0, xmm5
	vpcmpgeub k5, ymm2, ymm3
	vpcmpgeub k3, ymm7, ymm7
	vpcmpgeub k4, zmm4, zmm6
	vpcmpgeub k2, zmm4, zmm2

%ifdef ERROR
	vpcmpgeub k5, xmm14, xmm12
	vpcmpgeub k5, ymm15, ymm15
	vpcmpgeub k6, zmm12, zmm8
	vpcmpgeub k7, xmm19, xmm20
	vpcmpgeub k6, ymm25, ymm31
	vpcmpgeub k2, zmm28, zmm27
%endif
