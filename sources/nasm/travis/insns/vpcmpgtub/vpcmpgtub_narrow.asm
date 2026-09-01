	vpcmpgtub k7, xmm0, xmm3
	vpcmpgtub k4, xmm4, xmm6
	vpcmpgtub k4, ymm4, ymm4
	vpcmpgtub k3, ymm0, ymm5
	vpcmpgtub k4, zmm1, zword [0xd4a]
	vpcmpgtub k5, zmm1, zmm3

%ifdef ERROR
	vpcmpgtub k4, xmm14, xmm11
	vpcmpgtub k5, ymm12, ymm9
	vpcmpgtub k1, zmm15, zmm8
	vpcmpgtub k6, xmm27, xmm25
	vpcmpgtub k4, ymm26, ymm21
	vpcmpgtub k6, zmm29, zmm27
%endif
