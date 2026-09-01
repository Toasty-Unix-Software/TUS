	vpcmpnequw k3, xmm5, oword [0xa3e]
	vpcmpnequw k5, xmm2, xmm0
	vpcmpnequw k5, ymm5, yword [0x3c6]
	vpcmpnequw k4, ymm0, yword [0x1fd]
	vpcmpnequw k7, zmm6, zmm1
	vpcmpnequw k2, zmm0, zmm5

%ifdef ERROR
	vpcmpnequw k1, xmm11, xmm9
	vpcmpnequw k3, ymm8, ymm9
	vpcmpnequw k6, zmm10, zmm11
	vpcmpnequw k6, xmm18, xmm31
	vpcmpnequw k1, ymm25, ymm31
	vpcmpnequw k6, zmm17, zmm27
%endif
