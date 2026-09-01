	vpcmpltub k1, xmm5, oword [0xa43]
	vpcmpltub k1, xmm7, xmm0
	vpcmpltub k6, ymm4, ymm2
	vpcmpltub k3, ymm6, yword [0xa24]
	vpcmpltub k2, zmm5, zmm6
	vpcmpltub k3, zmm7, zword [0xe67]

%ifdef ERROR
	vpcmpltub k2, xmm12, xmm8
	vpcmpltub k3, ymm10, ymm12
	vpcmpltub k6, zmm12, zmm15
	vpcmpltub k5, xmm20, xmm21
	vpcmpltub k3, ymm27, ymm29
	vpcmpltub k1, zmm26, zmm19
%endif
