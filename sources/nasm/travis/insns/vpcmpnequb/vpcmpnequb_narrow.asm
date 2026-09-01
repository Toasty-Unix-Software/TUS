	vpcmpnequb k2, xmm5, xmm0
	vpcmpnequb k5, xmm6, xmm3
	vpcmpnequb k7, ymm6, ymm6
	vpcmpnequb k4, ymm0, ymm1
	vpcmpnequb k1, zmm5, zword [0xbea]
	vpcmpnequb k7, zmm4, zword [0x576]

%ifdef ERROR
	vpcmpnequb k2, xmm9, xmm10
	vpcmpnequb k1, ymm12, ymm14
	vpcmpnequb k6, zmm8, zmm9
	vpcmpnequb k7, xmm20, xmm29
	vpcmpnequb k7, ymm17, ymm20
	vpcmpnequb k3, zmm27, zmm23
%endif
