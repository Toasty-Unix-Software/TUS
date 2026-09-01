	vpcmpngtub k7, xmm5, xmm5
	vpcmpngtub k5, xmm7, oword [0xaa2]
	vpcmpngtub k7, ymm5, yword [0xc11]
	vpcmpngtub k7, ymm0, ymm2
	vpcmpngtub k3, zmm0, zmm3
	vpcmpngtub k1, zmm0, zword [0x7e0]

%ifdef ERROR
	vpcmpngtub k5, xmm9, xmm11
	vpcmpngtub k7, ymm11, ymm8
	vpcmpngtub k5, zmm11, zmm10
	vpcmpngtub k1, xmm26, xmm31
	vpcmpngtub k4, ymm18, ymm17
	vpcmpngtub k4, zmm20, zmm24
%endif
