	vpcmpequq k2, xmm4, xmm1
	vpcmpequq k3, xmm0, oword [0x302]
	vpcmpequq k2, ymm3, yword [0xd69]
	vpcmpequq k4, ymm2, ymm3
	vpcmpequq k4, zmm1, zmm6
	vpcmpequq k2, zmm6, zword [0x8bc]

%ifdef ERROR
	vpcmpequq k5, xmm10, xmm13
	vpcmpequq k1, ymm13, ymm13
	vpcmpequq k7, zmm10, zmm8
	vpcmpequq k2, xmm25, xmm18
	vpcmpequq k5, ymm27, ymm31
	vpcmpequq k7, zmm22, zmm27
%endif
