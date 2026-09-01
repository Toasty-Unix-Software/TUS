	vpcmpnleuw k6, xmm1, oword [0x860]
	vpcmpnleuw k3, xmm6, oword [0xdea]
	vpcmpnleuw k6, ymm6, ymm7
	vpcmpnleuw k5, ymm7, yword [0xbc8]
	vpcmpnleuw k2, zmm3, zmm1
	vpcmpnleuw k6, zmm4, zmm5

%ifdef ERROR
	vpcmpnleuw k1, xmm14, xmm10
	vpcmpnleuw k6, ymm12, ymm14
	vpcmpnleuw k5, zmm11, zmm9
	vpcmpnleuw k5, xmm26, xmm31
	vpcmpnleuw k5, ymm22, ymm27
	vpcmpnleuw k2, zmm29, zmm28
%endif
