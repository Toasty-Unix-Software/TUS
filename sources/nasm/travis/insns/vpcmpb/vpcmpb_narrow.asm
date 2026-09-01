	vpcmpb k4, xmm5, xmm0, 0xb5
	vpcmpb k5, xmm3, oword [0x43e], 0xc
	vpcmpb k3, ymm3, ymm3, 0xbb
	vpcmpb k7, ymm6, ymm6, 0xc0
	vpcmpb k1, zmm2, zmm4, 0xd2
	vpcmpb k6, zmm5, zmm3, 0x51

%ifdef ERROR
	vpcmpb k3, xmm15, xmm11, 0x3b
	vpcmpb k1, ymm14, ymm14, 0x74
	vpcmpb k4, zmm13, zmm13, 0xca
	vpcmpb k4, xmm29, xmm28, 0xbb
	vpcmpb k1, ymm31, ymm22, 0x48
	vpcmpb k2, zmm27, zmm29, 0xf7
%endif
