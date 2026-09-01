	vpcmpgeb k7, xmm0, xmm6
	vpcmpgeb k5, xmm7, oword [0xc19]
	vpcmpgeb k6, ymm6, ymm2
	vpcmpgeb k1, ymm6, ymm7
	vpcmpgeb k4, zmm6, zmm5
	vpcmpgeb k2, zmm6, zmm6

%ifdef ERROR
	vpcmpgeb k4, xmm12, xmm13
	vpcmpgeb k7, ymm8, ymm8
	vpcmpgeb k5, zmm15, zmm10
	vpcmpgeb k7, xmm31, xmm26
	vpcmpgeb k7, ymm21, ymm30
	vpcmpgeb k3, zmm18, zmm21
%endif
