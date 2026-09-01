	vpcmpngtuq k7, xmm3, oword [0x4a0]
	vpcmpngtuq k3, xmm2, xmm3
	vpcmpngtuq k3, ymm2, yword [0xf72]
	vpcmpngtuq k3, ymm6, ymm1
	vpcmpngtuq k7, zmm3, zmm5
	vpcmpngtuq k2, zmm3, zmm0

%ifdef ERROR
	vpcmpngtuq k7, xmm12, xmm8
	vpcmpngtuq k3, ymm13, ymm10
	vpcmpngtuq k1, zmm13, zmm15
	vpcmpngtuq k1, xmm28, xmm22
	vpcmpngtuq k6, ymm23, ymm28
	vpcmpngtuq k1, zmm30, zmm20
%endif
