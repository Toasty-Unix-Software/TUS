	vpcmpequw k3, xmm4, xmm4
	vpcmpequw k7, xmm0, oword [0x977]
	vpcmpequw k1, ymm2, ymm7
	vpcmpequw k7, ymm7, yword [0x1f1]
	vpcmpequw k2, zmm1, zmm6
	vpcmpequw k5, zmm4, zmm4

%ifdef ERROR
	vpcmpequw k3, xmm15, xmm13
	vpcmpequw k3, ymm12, ymm9
	vpcmpequw k1, zmm11, zmm12
	vpcmpequw k1, xmm29, xmm22
	vpcmpequw k6, ymm23, ymm26
	vpcmpequw k1, zmm28, zmm19
%endif
