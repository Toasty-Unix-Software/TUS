	vpcmpged k2, xmm2, xmm4
	vpcmpged k2, xmm5, xmm6
	vpcmpged k1, ymm0, yword [0xe7d]
	vpcmpged k3, ymm4, ymm3
	vpcmpged k4, zmm0, zword [0x996]
	vpcmpged k7, zmm4, zmm6

%ifdef ERROR
	vpcmpged k2, xmm14, xmm11
	vpcmpged k2, ymm14, ymm11
	vpcmpged k6, zmm8, zmm12
	vpcmpged k7, xmm18, xmm21
	vpcmpged k5, ymm20, ymm19
	vpcmpged k5, zmm18, zmm30
%endif
