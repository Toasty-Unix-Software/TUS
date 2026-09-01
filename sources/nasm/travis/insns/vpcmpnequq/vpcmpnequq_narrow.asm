	vpcmpnequq k5, xmm1, xmm7
	vpcmpnequq k6, xmm1, xmm6
	vpcmpnequq k2, ymm6, ymm6
	vpcmpnequq k3, ymm0, ymm6
	vpcmpnequq k6, zmm0, zmm0
	vpcmpnequq k5, zmm1, zword [0xda6]

%ifdef ERROR
	vpcmpnequq k4, xmm8, xmm8
	vpcmpnequq k4, ymm8, ymm10
	vpcmpnequq k7, zmm12, zmm13
	vpcmpnequq k2, xmm18, xmm27
	vpcmpnequq k1, ymm29, ymm17
	vpcmpnequq k6, zmm22, zmm23
%endif
