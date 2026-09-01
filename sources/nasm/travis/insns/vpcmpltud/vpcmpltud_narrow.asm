	vpcmpltud k5, xmm3, oword [0x256]
	vpcmpltud k6, xmm1, oword [0xd3d]
	vpcmpltud k1, ymm0, ymm4
	vpcmpltud k4, ymm7, ymm0
	vpcmpltud k4, zmm4, zmm0
	vpcmpltud k7, zmm5, zmm5

%ifdef ERROR
	vpcmpltud k4, xmm10, xmm11
	vpcmpltud k7, ymm11, ymm12
	vpcmpltud k4, zmm15, zmm13
	vpcmpltud k1, xmm21, xmm29
	vpcmpltud k7, ymm30, ymm21
	vpcmpltud k1, zmm19, zmm23
%endif
