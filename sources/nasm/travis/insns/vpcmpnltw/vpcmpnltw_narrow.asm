	vpcmpnltw k2, xmm2, xmm7
	vpcmpnltw k2, xmm4, xmm1
	vpcmpnltw k6, ymm6, ymm6
	vpcmpnltw k4, ymm1, ymm4
	vpcmpnltw k6, zmm6, zmm1
	vpcmpnltw k6, zmm5, zword [0x85b]

%ifdef ERROR
	vpcmpnltw k1, xmm13, xmm12
	vpcmpnltw k7, ymm9, ymm9
	vpcmpnltw k1, zmm10, zmm11
	vpcmpnltw k5, xmm24, xmm22
	vpcmpnltw k7, ymm29, ymm18
	vpcmpnltw k2, zmm23, zmm29
%endif
