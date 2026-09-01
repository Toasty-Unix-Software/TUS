	vpcmpw k4, xmm6, xmm7, 0xa2
	vpcmpw k7, xmm4, oword [0x3ef], 0x22
	vpcmpw k4, ymm3, ymm7, 0xab
	vpcmpw k7, ymm3, ymm4, 0x96
	vpcmpw k4, zmm5, zmm1, 0xc1
	vpcmpw k1, zmm1, zword [0x693], 0xc0

%ifdef ERROR
	vpcmpw k4, xmm9, xmm13, 0x74
	vpcmpw k5, ymm10, ymm12, 0x56
	vpcmpw k3, zmm13, zmm13, 0x89
	vpcmpw k2, xmm28, xmm23, 0xe8
	vpcmpw k5, ymm18, ymm22, 0xe3
	vpcmpw k2, zmm29, zmm17, 0x29
%endif
