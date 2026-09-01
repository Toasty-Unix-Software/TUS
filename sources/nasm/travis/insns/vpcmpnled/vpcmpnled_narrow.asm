	vpcmpnled k6, xmm5, xmm6
	vpcmpnled k5, xmm7, xmm4
	vpcmpnled k1, ymm7, ymm3
	vpcmpnled k5, ymm5, ymm4
	vpcmpnled k1, zmm4, zmm2
	vpcmpnled k7, zmm5, zmm5

%ifdef ERROR
	vpcmpnled k2, xmm8, xmm10
	vpcmpnled k3, ymm13, ymm8
	vpcmpnled k6, zmm9, zmm12
	vpcmpnled k2, xmm29, xmm17
	vpcmpnled k4, ymm25, ymm19
	vpcmpnled k5, zmm23, zmm22
%endif
